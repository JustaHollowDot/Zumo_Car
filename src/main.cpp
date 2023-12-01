#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include "Car/Car.h"

Zumo32U4ButtonA buttonA;
Car car;

void setup() {
    Serial.begin(9600);

    car.setup();
    ledGreen(true);

    // buttonA.waitForButton();
    // delay(500);

    ledGreen(false);

    car.setup_line_follower();

    ledRed(true);
}

void loop() {
    car.update();
    car.follow_line();
}