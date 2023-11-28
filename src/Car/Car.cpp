#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include "Car.h"

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
    motors.set_movement(Movement_creator::turn_degrees(imu.gyro.turn_angle.z + 360));

    while (motors.current_movement.action != Action::NO_ACTION) {
        update();
        lineSensor.calibrate_sensors();
    }
}

void Car::follow_line(bool force_setup) {
    /*
    if (force_setup or !lineSensor.check_if_initialized()) {
        if (lineSensor.check_if_initialized()) {
            lineSensor.reset();
        }

        setup_line_follower();
    }
     */

    constexpr float max_distance_from_center = 2000;
    constexpr float KP = 400 / max_distance_from_center;
    constexpr float KD = 50 * KP;
    int16_t speed_difference = abs(KP * lineSensor.distance_from_center + KD * (lineSensor.distance_from_center - lineSensor.last_distance_from_center));
    int16_t inner_motor_speed = (int16_t)motors.max_speed - speed_difference;
    int16_t outer_motor_speed = (int16_t)motors.max_speed;

    int16_t turn_radius = (int16_t)(CAR_WIDTH / 2) * (outer_motor_speed + inner_motor_speed) / (outer_motor_speed - inner_motor_speed);

    Serial.println(turn_radius);

    motors.set_movement(Movement_creator::turn_with_radius(turn_radius, imu.gyro.turn_angle.z + 360));
}

