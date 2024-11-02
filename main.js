// The GATT service and characteristics. These are used to recognise our ESP
const LED_SERVICE_UUID = "a3a671c0-a063-444d-86b9-34fd0255d897";
const LED_CHARACTERISTIC_UUID = "a3a671c1-a063-444d-86b9-34fd0255d897";

const BT_OPTIONS = {
  filters: [
    {
      services: [LED_SERVICE_UUID], // Our LED service
    },
  ],
  // acceptAllDevices: true
};

let led;
let server;

async function getLedState() {
  return (await led.readValue()).getUint8(0);
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

function writeCharacteristic(characteristic, value) {
  characteristic.writeValue(new Uint8Array([value]));
}

async function connectToESP() {
  let device = await connectBT(BT_OPTIONS);
  server = await getGATTServer(device);

  led = await getCharacteristic(
    server,
    LED_SERVICE_UUID,
    LED_CHARACTERISTIC_UUID,
  );

  console.debug(
    `Connected to ESP: ${device.name}. LED is currently ${(await led.readValue()).getUint8(0)}`,
  );

  document.getElementById("toggle_led").disabled = false;
  document.getElementById("connect").disabled = true;
  document.getElementById("disconnect").disabled = false;
  document.getElementById("device_name").innerText = device.name;

  updateLedStateFrontend();
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
}
