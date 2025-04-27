#include <WiFi.h>
#include <PubSubClient.h>
#include "QMI8658.h"

float acc[3], gyro[3];
unsigned int tim_count = 0;

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

  QMI8658_read_xyz(acc, gyro, &tim_count);

  // MQTT Message in JSON
  String payload = "{";
  payload += "\"acc\":[" + String(acc[0]) + "," + String(acc[1]) + "," + String(acc[2]) + "],";
  payload += "\"gyro\":[" + String(gyro[0]) + "," + String(gyro[1]) + "," + String(gyro[2]) + "]";
  payload += "}";

  client.publish("sensor/qmi8658", payload.c_str());
  Serial.println("Message transmit");
  Serial.println(payload);
  delay(1000);

}
