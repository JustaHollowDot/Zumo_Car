#ifndef ZUMO_ROBOT_LINE_FOLLOWER_H
#define ZUMO_ROBOT_LINE_FOLLOWER_H

#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include "Car/SensorArray/LineSensor/LineSensor.h"
#include "util/AverageData/AverageData.h"
#include "util/PID_controller/PID_controller.h"




/*
class LineFollower {
public:
    LineSensor *lineSensor;
    PID_controller pid_controller;

    static constexpr float max_distance_from_center = 2000;

    LineFollower(LineSensor *lineSensor) {
        this->lineSensor = lineSensor;

        constexpr float KP = 400 / max_distance_from_center;
        pid_controller = PID_controller(KP, KP * 50);
    }

    void reset_calibration() {
        lineSensor->reset();
    }

    void calibrate_sensors() {
        lineSensor->calibrate_sensors();
    }

    int16_t get_turn_radius_for_line_following() {
        int16_t error_from_line = (int16_t) pid_controller.get_pid_value(lineSensor->distance_from_center);
    }
};
*/


/*
void Car::setup_line_follower() {
    motors.set_movement(Movement_creator::turn_degrees(imu.gyro.turn_angle.z + 360));

    while (motors.current_movement.action != Action::NO_ACTION) {
        update();
        lineSensor.calibrate_sensors();
    }
}

void Car::follow_line(bool force_setup) {
    if (force_setup or !lineSensor.check_if_initialized()) {
        if (lineSensor.check_if_initialized()) {
            lineSensor.reset();
        }

        setup_line_follower();
    }


    constexpr float max_distance_from_center = 2000;
    constexpr float KP = 400 / max_distance_from_center;
    constexpr float KD = 50 * KP;
    int16_t speed_difference = abs(KP * lineSensor.distance_from_center + KD * (lineSensor.distance_from_center - lineSensor.last_distance_from_center));
    int16_t inner_motor_speed = (int16_t)motors.max_speed - speed_difference;
    auto  outer_motor_speed = (int16_t)motors.max_speed;

    int16_t turn_radius = (int16_t)(CAR_WIDTH / 2) * (outer_motor_speed + inner_motor_speed) / (outer_motor_speed - inner_motor_speed);

    motors.set_movement(Movement_creator::turn_with_radius(turn_radius, imu.gyro.turn_angle.z + 360));
}
*/

#endif // ZUMO_ROBOT_LINE_FOLLOWER_H