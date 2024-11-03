### Arduino Nano ESP32 Web Bluetooth API example
###### (quick draft)

Arduino becomes Bluetooth device that can be read and written to from a webinterface using the Web Bluetooth API.

Tested on Arduino Nano ESP32 & Chromium on Linux

**Leaphy light sensor:**
Connect Leaphy ambient light sensor through Leaphy Delphy or something else to analog pin 0 (D14)

* Enable `#enable-web-bluetooth` flag in `chrome://flags`. OR run chromium with `chromium-browser --enable-features=WebBluetooth`
* Flash `arduino.ino` sketch to Arduino.
* Run with:
```shell
$ npx live-server .
```
