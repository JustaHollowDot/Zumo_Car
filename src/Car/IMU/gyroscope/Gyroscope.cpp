#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include "Gyroscope.h"

Gyroscope::Gyroscope(Zumo32U4IMU *imu) {
    this->imu_g = imu;
}

void Gyroscope::reset()  {
    last_update = micros();
    turn_angle = 0;
}

void Gyroscope::update() {
    turn_rate = imu_g->g.z - gyro_offset[2];

    uint32_t m = micros();
    uint32_t dt = m - last_update;
    last_update = m;

    int64_t d = turn_rate * dt;

    turn_angle += d * 14680064 / 17578125;
}