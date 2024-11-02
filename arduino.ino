#include <ArduinoBLE.h>

//  define A GATT service and characteristic.
//  read: https://learn.adafruit.com/introduction-to-bluetooth-low-energy/gatt
#define LED_SERVICE_UUID        "a3a671c0-a063-444d-86b9-34fd0255d897"
#define LED_CHARACTERISTIC_UUID "a3a671c1-a063-444d-86b9-34fd0255d897"


BLEService ledService(LED_SERVICE_UUID);

// Create characteristic. Allow it to be read and written to.
BLEByteCharacteristic switchCharacteristic(LED_CHARACTERISTIC_UUID, BLERead | BLEWrite);

const int ledPin = LED_BUILTIN; // pin to use for the LED

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // set LED pin to output mode
  pinMode(ledPin, OUTPUT);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");

    while (1);
  }

  // Set Bluetooth name and add our LED service.
  BLE.setLocalName("Arduino Nano ESP32 LED");

  // Add the characteristic to the LED service
  ledService.addCharacteristic(switchCharacteristic);

  // connect the service to ble
  BLE.addService(ledService);

  // Advertise the service. This is essential because this is how we can later recognise our Bluetooth signal.
  BLE.setAdvertisedService(ledService);

  // Set the value of the characteristic to 0. Will be same as LED state
  switchCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("hello moto");
}

void loop() {
  // Define central device, aka the device ("master") that connects to the ESP ("slave") and controls the LED.
  BLEDevice central = BLE.central();

  // if a central is connected to us:
  if (central) {
    Serial.print("Central connected to me: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {

      // If central writes to characteristic, set led to the value they set.
      if (switchCharacteristic.written()) {
        if (switchCharacteristic.value()) { // 1
          Serial.println("LED on");
          digitalWrite(ledPin, HIGH); // on
        } else {                            // anything other than 1
          Serial.println(F("LED off"));
          digitalWrite(ledPin, LOW);  // off
        }
      }
    }

    // when central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
