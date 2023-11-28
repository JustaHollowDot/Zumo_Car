#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include "IMU.h"


void IMU::setup() {
    Wire.begin();
    imu.init();
    imu.enableDefault();
    imu.configureForTurnSensing();

    gyro.setup();
    acc.setup();
}

void IMU::update() {
    if (imu.gyroDataReady()) {
        imu.readGyro();
        gyro.update();
    }

    if (imu.accDataReady()) {
        imu.readAcc();
        Vector_3<double> current_angle = {
                radians(gyro.turn_angle.x),
                radians(gyro.turn_angle.y),
                radians(gyro.turn_angle.z)
        };

        acc.update(current_angle);

        /*
        Serial.println(acc.speed.x);
        Serial.println(acc.speed.y);
        Serial.println(acc.speed.z);
        Serial.println();
         */
    }

}