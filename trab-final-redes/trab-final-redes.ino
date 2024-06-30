/*
Implementação adaptada de:
LIRA, Filipe ; J. F. (2019b). Codigo blescan2. Disponível em: <
https://drive.google.com/file/d/1QoEx1skjy2hrNU1d2hvLgoUhQtAbMWZO/view >.
Acesso em: Abril 2024.
*/

#include <WiFi.h>
#include <PubSubClient.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

WiFiClient esp32Client;
PubSubClient clientMQTT(esp32Client);

unsigned long lastTimeChecked = 0;
const char* topic = "topic/ble/devices";
const long publishBreak = 20000;
const char* mqttBroker = "192.168.0.223";

BLEScan* pBLEScan;

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {

  int referenceRSSI = -69;  

  double calculateDistanceFromRSSI(int deviceRSSI) {
    int n = 2;  // Variar n de 2 até 3,5 normalmente.
    int referenceRSSI = -69;
    double exp = (referenceRSSI - deviceRSSI) / (10.0 * n);
    return pow(10, exp);
  }

  void onResult(BLEAdvertisedDevice device) {
    double deviceDistance = calculateDistanceFromRSSI(device.getRSSI());
    Serial.println(F((
        String("Dispositivo: ") + 
        String(device.getAddress().toString().c_str()) + 
        String("\nNome: ") + String(device.getName().c_str()) + 
        String("\nDistância: ") + String(deviceDistance, 4)
      ).c_str()));

    if (device.getRSSI() < referenceRSSI) {
      // Serial.println(advertisedDevice.getRSSI());
    }
    if (device.getRSSI() > referenceRSSI) {
      // Serial.println(advertisedDevice.getRSSI());
      lastTimeChecked = millis();
    }
  }
};

void setup() {
  Serial.begin(115200);
  connectWifi();
  clientMQTT.setServer(mqttBroker, 1883);
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);
}

void connectWifi() {
  const char* wifiName = "Mateus";
  const char* wifiPassword = "mahs12345";
  WiFi.begin(wifiName, wifiPassword);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println(F("Wi-Fi conectado"));
  Serial.println("IP ESP32: " + String(WiFi.localIP()));
}


void connectMQTT() {
  const char* ClientID = "mqttx_4ba90028";
  // const char* usuario = "mqtt_redes";
  // const char* senha = "!T&ste123";
  while (!clientMQTT.connected()) {
    clientMQTT.connect(ClientID);
    Serial.println(F("Conectado ao MQTT"));
  }
}

void loop() {
  if (!clientMQTT.connected()) {
    connectMQTT();
  }
  scanBLE();
  clientMQTT.loop();
}

void scanBLE() {
  int scanTime = 10;
  // Serial.println(F("Estou na scanBLE"));
  BLEScanResults foundDevices = pBLEScan->start(scanTime);
  int deviceCount = foundDevices.getCount();
  Serial.println(deviceCount);
  if (deviceCount) {
    publishScanResults(deviceCount);
  }
  for (uint32_t i = 0; i < deviceCount; i++) {
    BLEAdvertisedDevice device = foundDevices.getDevice(i);
    double deviceDistance = calculateDistanceFromRSSI(device.getRSSI());
    Serial.println(F((String("Dispositivo: ") + String(device.getAddress().toString().c_str()) + String(device.getName().c_str()) + String("\nDistância: ") + String(deviceDistance, 4)).c_str()));
    Serial.println("\n");
    publishScanResults(device.getName().c_str(), deviceDistance, device.getAddress().toString().c_str());
  }
}

double calculateDistanceFromRSSI(int deviceRSSI) {
  int n = 2;  // Variar n de 2 até 3,5 normalmente.
  int referenceRSSI = -69;
  double exp = (referenceRSSI - deviceRSSI) / (10.0 * n);
  return pow(10, exp);
}

void publishScanResults(int deviceCount) {
  // if (millis() - lastTimeChecked > publishBreak) {
  //   clientMQTT.publish(topic, String("Não há dispositivos no alcance!").c_str(), true);
  // } else {
    clientMQTT.publish(topic, String("Total de dispositivos no alcance: " + String(deviceCount)).c_str(), true);
  // }
}

void publishScanResults(const char* deviceName, double deviceDistance, const char* deviceAdress) {
  // if (millis() - lastTimeChecked > publishBreak) {
  //   clientMQTT.publish(topic, String("Não há dispositivos no alcance!").c_str(), true);
  // } else {
  clientMQTT.publish(topic, (
      String("IP do ESP32: ") + String(WiFi.localIP()) +
      String("\nDispositivo: ") + String(deviceName) +
      String("\nDistancia: ") + String(deviceDistance, 4) +
      String("\nEndereço Mac: ") + String(deviceAdress) + 
      String("\nIP do broker: ") + String(mqttBroker) + 
      String("\nPorta do Broker: 1883")).c_str(), true);
  // }
} 