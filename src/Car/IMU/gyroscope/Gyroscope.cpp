#include <Wire.h>
#include "Zumo32U4.h"
#include "Gyroscope.h"

Zumo32U4IMU imu;

const float radius = 39 / 2;
const float counts_per_rev = 909.7;
const float circumference = 2*3.1415*radius;

void TurnSensor::reset()  {
    last_update = micros();
    turn_angle = 0;
}

void TurnSensor::update() {
    imu.readGyro();

    turn_rate = imu.g.z - gyro_offset;

    uint32_t m = micros();
    uint32_t dt = m - last_update;
    last_update = m;

    int64_t d = turn_rate * dt;

    turn_angle += d * 14680064 / 17578125;
}

void TurnSensor::setup() {
    Wire.begin();
    imu.init();
    imu.enableDefault();
    imu.configureForTurnSensing();

    int32_t total = 0;
    for (int i = 0; i < 1024; i++) {
        while (!imu.gyroDataReady()) {}

        imu.readGyro();
        total += imu.g.z;
    }

    gyro_offset = total / 1024;

    reset();
}