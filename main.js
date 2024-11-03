// The GATT service and characteristics
const SERVICE_UUID = "a3a671c0-a063-444d-86b9-34fd0255d897";
const LED_CHARACTERISTIC_UUID = "a3a671c1-a063-444d-86b9-34fd0255d897";
const LIGHT_CHARACTERISTIC_UUID = "a3a671c2-a063-444d-86b9-34fd0255d897";
const RGB_LED_CHARACTERISTIC_UUID = "a3a671c3-a063-444d-86b9-34fd0255d897";


const BT_OPTIONS = {
  filters: [
    {
      services: [SERVICE_UUID], // Our service. Used to recognise our ESP
    },
  ],
  // acceptAllDevices: true
};

let led;
let light;
let server;
let rgbLed;

async function getLedState() {
  return (await led.readValue()).getUint8();
}

async function getLightAmount() {
  return (await light.readValue()).getUint8();
}

async function toggleLed() {
  if (await getLedState() === 1) {
    writeCharacteristic(led, 0);
  } else {
    writeCharacteristic(led, 1);
  }
  updateLedStateFrontend();
}

async function updateLedStateFrontend() {
  document.getElementById("led_state").innerText = await getLedState() ? "ON" : "OFF";
}

async function updateLightAmountFrontend() {
  document.getElementById("light_percentage").innerText = await getLightAmount()
}


function writeCharacteristic(characteristic, value) {
  characteristic.writeValue(new Uint8Array([value]));
}

async function connectToESP() {
  let device = await connectBT(BT_OPTIONS);
  server = await getGATTServer(device);

  led = await getCharacteristic(
    server,
    SERVICE_UUID,
    LED_CHARACTERISTIC_UUID,
  );

  light = await getCharacteristic(
    server,
    SERVICE_UUID,
    LIGHT_CHARACTERISTIC_UUID,
  );

  rgbLed = await getCharacteristic(
    server,
    SERVICE_UUID,
    RGB_LED_CHARACTERISTIC_UUID,
  );

  console.debug(
    `Connected to ESP: ${device.name}. LED is currently ${(await led.readValue()).getUint8(0)}. Light level is ${await getLightAmount()}%`,
  );

  // set RGB LED and color picker to same value.
  document.getElementById("color_picker").value = "#ff9900"
  await changeRGBLEDColor(hexToRgb("ff9900"));

  document.getElementById("toggle_led").disabled = false;
  document.getElementById("connect").disabled = true;
  document.getElementById("disconnect").disabled = false;
  document.getElementById("color_picker").disabled = false;
  document.getElementById("device_name").innerText = device.name;

  updateLedStateFrontend();
  while (server.connected) {
    await updateLightAmountFrontend();
  }
}

async function getCharacteristic(gattServer, serviceUUID, characteristicUUID) {
  let service = await gattServer.getPrimaryService(serviceUUID);
  return await service.getCharacteristic(characteristicUUID);
}

async function getGATTServer(device) {
  return await device.gatt.connect();
}

async function connectBT(options) {
  return await navigator.bluetooth.requestDevice(options);
}


// TODO: listen for disconnect events (for example when forced by OS)
async function disconnectESP() {
  server.disconnect();
  document.getElementById("toggle_led").disabled = true;
  document.getElementById("connect").disabled = false;
  document.getElementById("disconnect").disabled = true;
  document.getElementById("color_picker").disabled = true;
}


async function changeRGBLEDColor(color) {
  // console.log(new Uint8Array([color.r, color.g, color.b]))
  await rgbLed.writeValue(new Uint8Array([color.r, color.g, color.b]))
}
function hexToRgb(hex) {
  var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
  return result ? {
    r: parseInt(result[1], 16),
    g: parseInt(result[2], 16),
    b: parseInt(result[3], 16)
  } : null;
}

window.onload = function () {
  document.getElementById("color_picker").addEventListener("input", async function (event) {
    await changeRGBLEDColor(hexToRgb(event.target.value));
  })
}
