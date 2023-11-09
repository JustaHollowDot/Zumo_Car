#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include "Car/Car.h"

Car car;

void setup() {
    Serial.begin(9600);
    car.setup();
}

void loop() {
    car.update_sensors();
    Serial.println("test");
    delay(1000);
}