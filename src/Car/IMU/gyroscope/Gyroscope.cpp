#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include "Gyroscope.h"

Gyroscope::Gyroscope(Zumo32U4IMU *imu) {
    this->imu_g = imu;
}

void Gyroscope::setup() {
    uint16_t gyro_update_counter = 0;

    Vector_3<float> gyro_totals;


    uint16_t update_amount = 1024;
    for ( ; ; ) {
        if (imu_g->gyroDataReady()) {
            imu_g->readGyro();
            gyro_totals.x += imu_g->g.x / turn_1;
            gyro_totals.y += imu_g->g.y / turn_1;
            gyro_totals.z += imu_g->g.z / turn_1;

            gyro_update_counter++;
        }

        if (gyro_update_counter >= update_amount) {
            break;
        }
    }

    gyro_offset.x = gyro_totals.x / update_amount;
    gyro_offset.y = gyro_totals.y / update_amount;
    gyro_offset.z = gyro_totals.z / update_amount;
}

void Gyroscope::reset()  {
    last_update = micros();
    turn_angle = {0};
}

void Gyroscope::update() {
    turn_rate.x = (float)imu_g->g.x - gyro_offset.x;
    turn_rate.y = (float)imu_g->g.y - gyro_offset.y;
    turn_rate.z = (float)imu_g->g.z - gyro_offset.z;

    uint32_t m = micros();
    uint32_t dt = m - last_update;
    last_update = m;

    float d_x = turn_rate.x * dt / turn_1;
    float d_y = turn_rate.y * dt / turn_1;
    float d_z = turn_rate.z * dt / turn_1;

    turn_angle.x += d_x * gyro_digits_to_degrees_per_second;
    turn_angle.y += d_y * gyro_digits_to_degrees_per_second;
    turn_angle.z += d_z * gyro_digits_to_degrees_per_second;
}