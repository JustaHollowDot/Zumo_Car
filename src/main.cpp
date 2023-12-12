#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include "Car/Car.h"
#include "Car_Actions/LineFollower/LineFollower.h"

Zumo32U4ButtonA buttonA;
Car car;

Zumo32U4OLED display;

void setup() {
    Serial.begin(9600);

    car.setup();
    car.motors.set_movement(Movement_creator::turn_degrees(90));

    /*
    ledGreen(true);

    // buttonA.waitForButton();
    // delay(500);

    car.display.setup();

    String names[3] = {"A", "B", "C"};
    String values[3] = {"test", "kake", "mer"};
    String suffixes[3] = {"", "", ""};
    car.display.set_upper_menu_items(names, values, suffixes);

    String names2[4] = {"A", "B", "C", "D"};
    String values2[4] = {"test", "kake", "mer", "noe"};
    String suffixes2[4] = {"", "", "", ""};
    car.display.set_lower_menu_items(names2, values2, suffixes2);
     */


    // car.setup_line_follower();
}

void loop() {
    // car.update();
    // car.follow_line();

    // car.display.add_message("test display");
    // delay(1000);

    car.update();
    Serial.println(car.motors.rotations);
}