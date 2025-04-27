#include <WiFi.h>
#include "QMI8658.h"

const char* ssid = "wifiName";
const char* password = "wifiPassword";

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
}

void loop() {
  // put your main code here, to run repeatedly:

}
