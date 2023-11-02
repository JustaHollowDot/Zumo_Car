#include <Wire.h>
#include <Zumo32U4.h>
#include "Car.h"

bool Car::update() {
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
            /* code */
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
    /* code */
}

void Car::turn_degrees() {
    if (current_movement.move_degrees > 0) {
        move(-turn_speed, turn_speed);
    } else {
        move(turn_speed, -turn_speed);
    }

    turn_sensor.update();


    if (current_movement.move_degrees > 0) {
        if (turn_sensor.turn_angle >= current_movement.move_degrees) {
            current_movement.action = Action::NO_ACTION;

            stop_movement();
        }
    } else {
        if (turn_sensor.turn_angle <= current_movement.move_degrees) {
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

    turn_sensor.update();
    if (current_movement.move_degrees < 0) {
        if (turn_sensor.turn_angle <= current_movement.move_degrees) {
            current_movement.action = Action::NO_ACTION;

            stop_movement();
        }
    } else {
        if (turn_sensor.turn_angle >= current_movement.move_degrees) {
            current_movement.action = Action::NO_ACTION;

            stop_movement();
        }
    }
}

void Car::follow_line() {
    /* code */
}

void Car::calibrate_line(bool line_is_black) {
    /* code */
}

void Car::set_move(int16_t distance = 0, int16_t time = 0, bool drive_forward = true) {
    if (distance) {
        current_movement = {
                .action = Action::MOVE_DISTANCE,
                .time_initialized = millis(),
                .move_forward = drive_forward,
                .move_time = 0,
                .move_distance = distance,
                .move_degrees = 0,
                .move_radius = 0
        };
    } else {
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
}

void Car::set_turn_degrees(int32_t degrees) {
    current_movement = {
            .action = Action::TURN_DEGREES,
            .time_initialized = millis(),
            .move_forward = false,
            .move_time = 0,
            .move_distance = 0,
            .move_degrees = degrees * turn_sensor.turn_1,
            .move_radius = 0,
    };

    if (!turn_sensor_setup) {
        turn_sensor.setup();
        turn_sensor_setup = true;
    }

    turn_sensor.reset();
}

void Car::set_turn_with_radius(int16_t radius, int32_t degrees) {
    current_movement = {
            .action = Action::TURN_WITH_RADIUS,
            .time_initialized = millis(),
            .move_forward = false,
            .move_time = 0,
            .move_distance = 0,
            .move_degrees = degrees * turn_sensor.turn_1,
            .move_radius = radius,
    };

    if (!turn_sensor_setup) {
        turn_sensor.setup();
        turn_sensor_setup = true;
    }

    turn_sensor.reset();
}

void Car::set_follow_line(bool line_is_black) {
    /* code */
}