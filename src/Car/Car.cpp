#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include "Car.h"
#include "util/PID_controller/PID_controller.h"

constexpr float max_distance_from_center = 2000;
PID_controller pidController = PID_controller(1.0 / 3.0, 8.0, 0);

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
    battery.drain_from_velocity(motors.speed);
    battery.update();
}
