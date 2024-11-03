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

### TODO:
The device should have a [Device information Service](https://www.bluetooth.com/specifications/specs/dis-1-2/). As many of its required [characteristics](https://www.bluetooth.com/wp-content/uploads/Files/Specification/HTML/DIS_v1.2/out/en/index-en.html#UUID-11cbfd5c-b7fa-3d12-0f0c-3fdbbd2e4efb) should be implemented.
This is useful because it enables external systems to access device information (like model numbers, etc)


### Resources
* https://webbluetoothcg.github.io/web-bluetooth/
* https://github.com/WebBluetoothCG/registries
* https://docs.arduino.cc/libraries/arduinoble
