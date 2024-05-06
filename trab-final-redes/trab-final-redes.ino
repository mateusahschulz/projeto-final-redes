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
    Serial.println(F((String("Dispositivo: ") + String(device.getAddress().toString().c_str()) + String("\nNome: ") + String(device.getName().c_str()) + String("\nDistância: ") + String(deviceDistance, 4)).c_str()));

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
  const char* mqttBroker = "test.mosquitto.org";
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
  const char* wifiName = "AHSCHULZ.2G";
  const char* wifiPassword = "7082D154";
  WiFi.begin(wifiName, wifiPassword);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println(F("Wi-Fi conectado"));
}


void connectMQTT() {
  // const char* ClientID = "mqtt_trab_final_redes_02_05_2024";
  // const char* usuario = "mqtt_redes";
  // const char* senha = "!T&ste123";
  while (!clientMQTT.connected()) {
    clientMQTT.connect("");
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
    // Serial.println(F((String("Dispositivo: ") + String(device.getAddress().toString().c_str()) + String(device.getName().c_str()) + String("\nDistância: ") + String(deviceDistance, 4)).c_str()));
    publishScanResults(device.getName().c_str(), deviceDistance);
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

void publishScanResults(const char* deviceName, double deviceDistance) {
  // if (millis() - lastTimeChecked > publishBreak) {
  //   clientMQTT.publish(topic, String("Não há dispositivos no alcance!").c_str(), true);
  // } else {
    clientMQTT.publish(topic, (String("Dispositivo: ") + String(deviceName) + String("\nDistância: ") + String(deviceDistance, 4)).c_str(), true);
  // }
} 