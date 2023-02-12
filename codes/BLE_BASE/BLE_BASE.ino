//==============================================================================
// Includes
//==============================================================================

#include <ArduinoBLE.h>
#include "Nicla_System.h"
#include "Arduino_BHY2.h"

//==============================================================================
// LED / State status functions
//==============================================================================


void rgbLedRed()
{
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, LOW);
  digitalWrite(LEDB, LOW);
}

void rgbLedGreen()
{
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, LOW);
}

void setupLED() {
  // Prepare LED pins.
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
}


//==============================================================================
// BLE
//==============================================================================

#define LOCAL_NAME "LalotestNICLA" //Change the name
#define FLOAT_BYTE_SIZE 4
#define UUID_GEN(val) ("81c30e5c-" val "-4f7d-a886-de3e90749161")

BLEService service (UUID_GEN("1101"));
BLEUnsignedCharCharacteristic gestureDataCharacteristic (UUID_GEN("2101"), BLERead | BLENotify);

void setupBLE() {
  service.addCharacteristic(gestureDataCharacteristic);

  // Start the core BLE engine.
  if (!BLE.begin())
  {
    Serial.println("Failed to initialized BLE!");
    //showErrorLed();
    //while (1) showErrorLed();
  }

  String address = BLE.address();

  // Output BLE settings over Serial.
  Serial.print("address = ");
  Serial.println(address);

  address.toUpperCase();

  static String deviceName = LOCAL_NAME;
  deviceName += " - ";
  deviceName += address[address.length() - 5];
  deviceName += address[address.length() - 4];
  deviceName += address[address.length() - 2];
  deviceName += address[address.length() - 1];

  Serial.print("deviceName = ");
  Serial.println(deviceName);

  Serial.print("localName = ");
  Serial.println(deviceName);

  // Set up properties for the whole service.
  BLE.setLocalName(deviceName.c_str());
  BLE.setDeviceName(deviceName.c_str());
  BLE.addService(service);
  BLE.setAdvertisedService(service);

  // Print out full UUID and MAC address.
  Serial.println("Peripheral advertising info: ");
  Serial.print("Name: ");
  Serial.println(LOCAL_NAME);
  Serial.print("MAC: ");
  Serial.println(BLE.address());
  Serial.print("Service UUID: ");
  Serial.println(service.uuid());

  // Start up the service itself.
  
  BLE.advertise();

  Serial.println("Bluetooth device active, waiting for connections...");
}

//==============================================================================
// Setup / Loop
//==============================================================================

void setup() {        
  Serial.begin(115200);
  setupLED();
  setupBLE();
  //rgbLedRed();
  rgbLedGreen();
}

void loop() {  
  // if a central is connected to the peripheral:
  static bool wasConnected = false;  
  bool isConnected = BLE.connected();
  wasConnected = isConnected;
  if (isConnected) { //change this part to send data using BLE to python
    int battery = analogRead(A0);
    Serial.print("Battery Level % is now: ");
    Serial.println(battery);
    gestureDataCharacteristic.writeValue(battery);
    delay(500);
  }
}
