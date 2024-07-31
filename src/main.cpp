#include <Arduino.h>
#include "BLEFunctions.h"

// https://github.com/marcboeker/esp32-ble-ios-demo/blob/master/esp32/src/main.cpp
//https://forum.arduino.cc/t/esp-32-ble-notify-on-change-does-not-notify/1026292


BLEHandler bleHandler;


void setup() {
  Serial.begin(115200);
  Serial.printf("Test de BLE. Fecha y hora de comp: %s, %s\n", __DATE__, __TIME__);
  Serial.println("En la App nfConnect, en la característica");
  Serial.println("La flecha hacia abajo que tiene la raya");
  Serial.println("Para que se actualice automáticamente");
  bleHandler.init();
}

void loop() {
  delay(1000);
  bleHandler.update();
  
}

