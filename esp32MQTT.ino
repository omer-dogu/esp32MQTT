#include "QMI8658.h"

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
}

void loop() {
  // put your main code here, to run repeatedly:

}
