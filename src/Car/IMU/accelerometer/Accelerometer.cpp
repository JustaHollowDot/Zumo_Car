#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include "Accelerometer.h"

Accelerometer::Accelerometer(Zumo32U4IMU imu) {
    this->imu_a = imu;
}

void Accelerometer::reset() {
    last_update = micros();

    acceleration.reset();
    speed.reset();
    position.reset();
}

void Accelerometer::update() {
    get_acceleration();
    calculate_speed();
    calculate_position();
}

void Accelerometer::get_acceleration() {
    acceleration.x.add_data(imu_a.a.x);
    acceleration.y.add_data(imu_a.a.y);
    acceleration.z.add_data(imu_a.a.z);
}

void Accelerometer::calculate_speed() {
    speed += acceleration;
}

void Accelerometer::calculate_position() {
    position += speed;
}

