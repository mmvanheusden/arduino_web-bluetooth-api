### Arduino Nano ESP32 Web Bluetooth API example
###### (quick draft)

Arduino becomes Bluetooth device that can be read and written to from a webinterface using the Web Bluetooth API.

Tested on Arduino Nano ESP32 & Chromium on Linux

* Enable `#enable-web-bluetooth` flag in `chrome://flags`.
* Flash `arduino.ino` sketch to Arduino.
* Run with:
```shell
$ npx live-server .
```
