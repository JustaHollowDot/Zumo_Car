#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include "Car/Car.h"

Zumo32U4ButtonA buttonA;
Zumo32U4OLEDCore oledCore;
Zumo32U4OLED oled;

Car car;

void setup() {
    Serial.begin(9600);

    car.setup();
    ledGreen(true);

    buttonA.waitForButton();
    delay(500);

    car.setup_line_follower();
}

void loop() {
    car.update();
    car.follow_line();
    ledGreen(false);

    Serial.println(car.lineSensor.distance_from_center);
    Serial.println();

}