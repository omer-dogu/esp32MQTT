#include <WiFi.h>
#include <PubSubClient.h>
#include "QMI8658.h"

const char* ssid = "wifiName";
const char* password = "wifiPassword";
const char* mqtt_broker = "127.0.0.1";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Connecting MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("Connected MQTT!");
    } else {
      Serial.print("Error: ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  if(psramInit())
    Serial.println("\nPSRAM is correctly initialized");
  else
    Serial.println("PSRAM not available");
  
  if (DEV_Module_Init() != 0)
    Serial.println("GPIO Init Fail!");
  else
    Serial.println("GPIO Init successful!");

  QMI8658_init();

  setup_wifi();
  client.setServer(mqtt_broker, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

}
