#include <ArduinoBLE.h>
#include <math.h>

//  define A GATT service and characteristic.
//  read: https://learn.adafruit.com/introduction-to-bluetooth-low-energy/gatt
#define SERVICE_UUID        "a3a671c0-a063-444d-86b9-34fd0255d897"
#define LED_CHARACTERISTIC_UUID "a3a671c1-a063-444d-86b9-34fd0255d897"
#define LIGHT_CHARACTERISTIC_UUID "a3a671c2-a063-444d-86b9-34fd0255d897"
#define RGB_CHARACTERISTIC_UUID "a3a671c3-a063-444d-86b9-34fd0255d897"



const int LED_PIN = LED_BUILTIN; // pin to use for the LED
const int LEAPHY_AMBIENT_LIGHT_SENSOR = A0;


BLEService arduinoService(SERVICE_UUID);

// Create characteristics, set read/write permissions
BLEBoolCharacteristic ledCharacteristic(LED_CHARACTERISTIC_UUID, BLERead | BLEWrite);
BLECharacteristic rgbCharacteristic(RGB_CHARACTERISTIC_UUID, BLEWrite | BLERead, 3); // length of 3 bytes
BLEByteCharacteristic lightCharacteristic(LIGHT_CHARACTERISTIC_UUID, BLERead);


void setup() {
  Serial.begin(9600);
  // while (!Serial); uncomment to wait for serial monitor

  // Set pinmodes  
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_PIN, OUTPUT);


  // begin initialization
  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");

    while (1);
  }

  // Set Bluetooth name
  BLE.setLocalName("Arduino Nano ESP32");

  // Add the characteristics to the service
  arduinoService.addCharacteristic(ledCharacteristic);
  arduinoService.addCharacteristic(lightCharacteristic);
  arduinoService.addCharacteristic(rgbCharacteristic);



  // add our service
  BLE.addService(arduinoService);

  // Advertise the service. This is essential because this is how we can later recognise our Bluetooth signal.
  BLE.setAdvertisedService(arduinoService);


  // Turn LED off and set LED characteristic to 0
  digitalWrite(LED_PIN, LOW);
  ledCharacteristic.writeValue(false);

  // Turn RGB LED off and set its characteristic
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, HIGH);
  byte rgb_off[3] = {0, 0, 0};
  rgbCharacteristic.writeValue(rgb_off, 3);

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
      int sensorValue = analogRead(LEAPHY_AMBIENT_LIGHT_SENSOR);
      // Max value sensor gives is 4096.
      int lightPercentage = (round(sensorValue) / 4096 * 100);

      // Print out the value you read:
      // Serial.printf("Leaphy light sensor reads: ");
      // Serial.print(lightPercentage);
      // Serial.println("%");
      lightCharacteristic.writeValue(lightPercentage);
      

      // If central writes to characteristic, set led to the value they set.
      if (ledCharacteristic.written()) {
        if (ledCharacteristic.value()) { // 1
          Serial.println("LED on");
          digitalWrite(LED_PIN, HIGH); // on
        } else {                            // anything other than 1
          Serial.println(F("LED off"));
          digitalWrite(LED_PIN, LOW);  // off
        }
      }


      if (rgbCharacteristic.written()) {
          // Serial.println(rgbCharacteristic.value()[0]);
          // Serial.println(rgbCharacteristic.value()[1]);
          // Serial.println(rgbCharacteristic.value()[2]);

          
          // for some reason 255 is off, 0 is on
          analogWrite(LED_RED, (255 - rgbCharacteristic.value()[0]));
          analogWrite(LED_GREEN, (255 - rgbCharacteristic.value()[1]));
          analogWrite(LED_BLUE, (255 - rgbCharacteristic.value()[2]));
      }
    }

    // when central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
