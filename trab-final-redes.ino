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
const char* topic = "Busca - ";
const long publishBreak = 20000;

BLEScan* pBLEScan;

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {

  int referenceRSSI = -69;

  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.getRSSI() < referenceRSSI) {
      Serial.println(advertisedDevice.getRSSI());
    }
    if (advertisedDevice.getRSSI() > referenceRSSI) {
      Serial.println(advertisedDevice.getRSSI());
      lastTimeChecked = millis();
    }
  }
};

void setup() {
  const char* mqttBroker = "postman.cloudmqtt.com";
  Serial.begin(115200);
  connectWifi();
  clientMQTT.setServer(mqttBroker, 11458);
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);
}

void connectWifi() {
  /*Serial.println(F("Insira a rede wi-fi que deseja se conectar: "));
  const char* wifiName = Serial.readString().c_str();
  Serial.println(F("Insira a senha da rede para conexão: "));
  const char* wifiPassword = Serial.readString().c_str();*/
  const char* wifiName = "AHSCHULZ.2G";
  const char* wifiPassword = "7082D154";
  WiFi.begin(wifiName, wifiPassword);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println(F("Wi-Fi conectado"));
}


void connectMQTT() {
  const char* ClientID = "ESP32";
  const char* usuario = "zsaameag";
  const char* senha = "L9R3_C1ms2Lb";
  while (!clientMQTT.connected()) {
    clientMQTT.connect(ClientID, usuario, senha);
    Serial.println(F("Conectado ao MQTT"));
  }
}

void loop() {
  if (!clientMQTT.connected()) {
    connectMQTT();
  }
  scanBLE();
}

void scanBLE() {
  int scanTime = 2;
  BLEScanResults foundDevices = pBLEScan->start(scanTime);
  int deviceCount = foundDevices.getCount();
  if (deviceCount) {
    publishScanResults(deviceCount);
  }
  for (uint32_t i = 0; i < deviceCount; i++) {
    BLEAdvertisedDevice device = foundDevices.getDevice(i);
    double deviceDistance = calculateDistanceFromRSSI(device.getRSSI());
    publishScanResults(device.getName().c_str(), deviceDistance);
  }
}

double calculateDistanceFromRSSI(int deviceRSSI) {
  int n = 2;  // Variar n de 2 até 3,5 normalmente.
  int referenceRSSI = -9;
  float exp = (deviceRSSI - referenceRSSI) / (10 * n);
  return pow(10, exp);
}

void publishScanResults(int deviceCount) {
  if (millis() - lastTimeChecked > publishBreak) {
    clientMQTT.publish(topic, String("Não há dispositivos no alcance!").c_str(), true);
  } else {
    clientMQTT.publish(topic, String("Total de dispositivos no alcance: " + deviceCount).c_str(), true);
  }
}

void publishScanResults(const char* deviceName, double deviceDistance) {
  if (millis() - lastTimeChecked > publishBreak) {
    clientMQTT.publish(topic, String("Não há dispositivos no alcance!").c_str(), true);
  } else {
    clientMQTT.publish(topic, (String("Dispositivo: ") + String(deviceName) + String("\nDistância: ") + String(deviceDistance, 2)).c_str(), true);
  }
} 