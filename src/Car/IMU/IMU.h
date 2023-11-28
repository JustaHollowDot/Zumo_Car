#ifndef ZUMO_ROBOT_IMU_H
#define ZUMO_ROBOT_IMU_H

#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include "accelerometer/Accelerometer.h"
#include "gyroscope/Gyroscope.h"

class IMU {
public:
    Zumo32U4IMU imu;

    Accelerometer acc = Accelerometer(&imu);
    Gyroscope gyro = Gyroscope(&imu);

    IMU() = default;
    void setup();
    void update();

    void calculate_speed();
    void calculate_position();
    void calculate_angle();
};

#endif //ZUMO_ROBOT_IMU_H