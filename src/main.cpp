#include <Arduino.h>
#include <Wire.h>
#include <Zumo32u4.h>
#include <Car.h>

Car car;

void setup() {
    Serial.begin(9600);
}

void loop() {
    for (int i = 0; i < 4; i++) {
        car.set_turn_with_radius(100, 90);

        while (!car.update());
    }

    for (int i = 0; i < 4; i++) {
        car.set_turn_with_radius(100, -90);

        while (!car.update());
    }

    delay(500);

    car.set_turn_degrees(180);

    while (!car.update());

    delay(500);
}
