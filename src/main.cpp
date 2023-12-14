#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include "Car/Car.h"
#include "Car_Actions/LineFollower/LineFollower.h"
#include "Car_Actions/SimpleMovements/SimpleMovements.h"

Car car;
LineFollower line_follower(&car);
SimpleMovements simple_movements(&car);

enum CarAction {
    LINEFOLLOWER,
    SIMPLEMOVEMENTS,
    OTHER
};

CarAction action = CarAction::LINEFOLLOWER;

void setup() {
    Serial.begin(9600);

    car.setup();


    if (action == CarAction::LINEFOLLOWER) {
        line_follower.setup();

        ledGreen(true);

        line_follower.explore_maze();
    }

    if (action == CarAction::SIMPLEMOVEMENTS) {
        simple_movements.drive_rectangle();
        ledGreen(true);

        simple_movements.drive_circle();
        ledRed(true);

        simple_movements.drive_forward_and_back();
        ledYellow(true);

        simple_movements.drive_between_obstacles(100, 5);
        ledGreen(false);
    }

    // car.motors.set_movement(Movement_creator::turn_degrees(90));
}

void loop() {
    if (action == CarAction::LINEFOLLOWER) {
        line_follower.drive_loop();
    }

    if (action == CarAction::OTHER) {
        car.update();

        // car.follow_line();

        // car.display.add_message("test display");
        // delay(1000);

        // car.update();
        // Serial.println(car.motors.rotations);
    }
}