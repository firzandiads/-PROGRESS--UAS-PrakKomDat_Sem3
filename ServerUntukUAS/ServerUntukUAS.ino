/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "0e206fa8-eb88-46f3-81d8-7178247ab124"
#define CHARACTERISTIC_UUID "c920d5a2-fac9-4063-86e6-f48ca1da65dc"
#define CHARACTERISTIC1_UUID "c7d5cdfa-c2fd-43f4-8e9f-b7e08d6598cf" // konekin
#define echoPin 18 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 5 //attach pin D3 Arduino to pin Trig of HC-SR04

// defines variables
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement

BLEService* pService;
BLECharacteristic* pCharacteristic;
BLECharacteristic* mCharacteristic;
BLEServer* pServer;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT

  BLEDevice::init("Firzandi Rezha");
  pServer = BLEDevice::createServer();
  pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ
                                       );
   mCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC1_UUID,
                                         BLECharacteristic::PROPERTY_READ
                                       );

  pCharacteristic->setValue("Firzandi");
  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0){
    // read the incoming byte:
    String incomingByte = Serial.readString();
    char txString[(incomingByte.length()+1)];
    incomingByte.toCharArray(txString,incomingByte.length());

    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte);
    pCharacteristic->setValue(txString);
    pCharacteristic->notify();
  }
  delay(1000);
  read_jarak();
}
void read_jarak() {
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  Serial.print(distance);
  Serial.println(" CM");
  String yeuh ="";
  yeuh.concat(distance);
  Serial.print(yeuh);
  Serial.println(" CM");
  char txString[(yeuh.length()+1)];
   yeuh.toCharArray(txString,yeuh.length()+1);
    mCharacteristic->setValue(txString);
    mCharacteristic->notify();
  delay(1000);
}
