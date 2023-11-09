#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include "Car.h"

void Car::setup() {
    imu.setup();
}

// add to car.update();
void Car::update_sensors() {
    imu.update();
}


// change to return enum result to differentiate the different points of failure
bool Car::update() {
    // change to motors.update()
    switch (current_movement.action) {
        case Action::MOVE_TIME:
            move_time();
            break;

        case Action::MOVE_DISTANCE:
            move_distance();
            break;

        case Action::TURN_DEGREES:
            turn_degrees();
            break;

        case Action::TURN_WITH_RADIUS:
            turn_with_radius();
            break;

        case Action::FOLLOW_LINE:
            break;

        default:
            stop_movement();

            return true;
    }

    return false;
}

void Car::move(int16_t left_speed, int16_t right_speed) {
    Zumo32U4Motors::setSpeeds(left_speed, right_speed);
}

void Car::stop_movement() {
    move(0, 0);
}

void Car::move_time() {
    if (current_movement.move_forward) {
        move(max_speed, max_speed);
    } else {
        move(-max_speed, -max_speed);
    }

    if (current_movement.time_initialized < millis() - 1000) {
        current_movement.action = Action::NO_ACTION;
    }
}

void Car::move_distance() {
}

void Car::turn_degrees() {
    if (current_movement.move_degrees > 0) {
        move(-turn_speed, turn_speed);
    } else {
        move(turn_speed, -turn_speed);
    }

    imu.gyro.update();


    if (current_movement.move_degrees > 0) {
        if (imu.gyro.turn_angle >= current_movement.move_degrees) {
            current_movement.action = Action::NO_ACTION;

            stop_movement();
        }
    } else {
        if (imu.gyro.turn_angle <= current_movement.move_degrees) {
            current_movement.action = Action::NO_ACTION;

            stop_movement();
        }
    }
}

void Car::turn_with_radius() {
    double radius_1 = current_movement.move_radius - 49;
    double radius = current_movement.move_radius + 49;
    double diff_r = radius / radius_1;

    if (current_movement.move_degrees < 0) {
        move((int)(max_speed*diff_r), max_speed);
    } else {
        move(max_speed, (int)(max_speed*diff_r));
    }

    imu.gyro.update();
    if (current_movement.move_degrees < 0) {
        if (imu.gyro.turn_angle <= current_movement.move_degrees) {
            current_movement.action = Action::NO_ACTION;

            stop_movement();
        }
    } else {
        if (imu.gyro.turn_angle >= current_movement.move_degrees) {
            current_movement.action = Action::NO_ACTION;

            stop_movement();
        }
    }
}

void Car::follow_line() {
}

void Car::calibrate_line(bool line_is_black) {
}

void Car::set_move_time(int16_t time, bool drive_forward) {
    current_movement = {
            .action = Action::MOVE_TIME,
            .time_initialized = millis(),
            .move_forward = drive_forward,
            .move_time = time,
            .move_distance = 0,
            .move_degrees = 0,
            .move_radius = 0
    };
}

void Car::set_move_distance(int16_t distance, bool drive_forward) {
    current_movement = {
            .action = Action::MOVE_DISTANCE,
            .time_initialized = millis(),
            .move_forward = drive_forward,
            .move_time = 0,
            .move_distance = distance,
            .move_degrees = 0,
            .move_radius = 0
    };
}

void Car::set_turn_degrees(int32_t degrees) {
    current_movement = {
            .action = Action::TURN_DEGREES,
            .time_initialized = millis(),
            .move_forward = false,
            .move_time = 0,
            .move_distance = 0,
            .move_degrees = degrees * imu.gyro.turn_1,
            .move_radius = 0,
    };

    if (!turn_sensor_setup) {
        imu.gyro.setup();
        turn_sensor_setup = true;
    }

    imu.gyro.reset();
}

void Car::set_turn_with_radius(int16_t radius, int32_t degrees) {
    current_movement = {
            .action = Action::TURN_WITH_RADIUS,
            .time_initialized = millis(),
            .move_forward = false,
            .move_time = 0,
            .move_distance = 0,
            .move_degrees = degrees * imu.gyro.turn_1,
            .move_radius = radius,
    };

    if (!turn_sensor_setup) {
        imu.gyro.setup();
        turn_sensor_setup = true;
    }

    imu.gyro.reset();
}

void Car::set_follow_line(bool line_is_black) {
}