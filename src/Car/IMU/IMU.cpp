#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include "IMU.h"

void IMU::setup() {
    Wire.begin();
    imu.init();
    imu.enableDefault();

    uint16_t gyro_update_counter = 0;
    uint16_t acc_update_counter = 0;

    int32_t gyro_totals[3] = {0};
    int32_t acc_totals[3] = {0};


    uint16_t update_amount = 1024;
    for ( ; ; ) {
        if (gyro_update_counter < update_amount and !imu.gyroDataReady()) {
            imu.readGyro();
            gyro_totals[0] = imu.g.x;
            gyro_totals[1] = imu.g.y;
            gyro_totals[2] = imu.g.z;

            gyro_update_counter++;
        }

        if (acc_update_counter < update_amount and !imu.accDataReady()) {
            imu.readAcc();
            acc_totals[0] = imu.a.x;
            acc_totals[1] = imu.a.y;
            acc_totals[2] = imu.a.z;

            acc_update_counter++;
        }

        if (gyro_update_counter >= update_amount and acc_update_counter >= update_amount) {
            gyro.gyro_offset[0] = gyro_totals[0] / update_amount;
            gyro.gyro_offset[1] = gyro_totals[1] / update_amount;
            gyro.gyro_offset[2] = gyro_totals[2] / update_amount;

            acc.acc_offset[0] = acc_totals[0] / update_amount;
            acc.acc_offset[1] = acc_totals[1] / update_amount;
            acc.acc_offset[2] = acc_totals[2] / update_amount;

            break;
        }
    }
}

void IMU::update() {
    bool acc_ready = imu.accDataReady(), gyro_ready = imu.gyroDataReady();

    imu.read();

    if (acc_ready) {
        acc.update();
    }

    if (gyro_ready) {
        gyro.update();
    }
}