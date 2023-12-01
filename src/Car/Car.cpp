#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include "Car.h"
#include "util/PID_controller/PID_controller.h"

constexpr float max_distance_from_center = 2000;
PID_controller pidController = PID_controller(0.4, 5, 0.2);

void Car::setup() {
    lineSensor.init();
    imu.setup();
    motors.setup();
}

void Car::update() {
    lineSensor.update();
    imu.update();
    motors.set_turn_angle(imu.gyro.turn_angle.z);
    motors.update();
}


void Car::setup_line_follower() {
    if (lineSensor.check_if_initialized()) {
        lineSensor.reset();
    }

    imu.gyro.reset();
    motors.set_movement(Movement_creator::turn_degrees(imu.gyro.turn_angle.z + 360));

    while (motors.current_movement.action != Action::NO_ACTION) {
        update();
        lineSensor.calibrate_sensors();
    }
}

void Car::follow_line() {
    int16_t speed_difference = pidController.get_pid_value(lineSensor.distance_from_center);
    int16_t left_speed;
    int16_t right_speed;

    if (speed_difference < 0) {
        left_speed = motors.max_speed - abs(speed_difference);
        right_speed = motors.max_speed;
    } else {
        left_speed = motors.max_speed;
        right_speed = motors.max_speed - abs(speed_difference);
    }

    left_speed = constrain(left_speed, 0, (int16_t)motors.max_speed);
    right_speed = constrain(right_speed, 0, (int16_t)motors.max_speed);

    motors.set_movement(Movement_creator::move(left_speed, right_speed));

    /*
    int16_t turn_radius;

    // division by 0 returns 0, but should return int_max ('infinite' radius)
    if (left_speed == right_speed) {
        turn_radius = (CAR_WIDTH / 2) * (right_speed + left_speed) / (right_speed - left_speed);
    } else {
        turn_radius = (uint16_t)-1 >> 1;
    }

    Serial.println(turn_radius);

    motors.set_movement(Movement_creator::turn_with_radius(turn_radius, imu.gyro.turn_angle.z + 360, false));
    */
}