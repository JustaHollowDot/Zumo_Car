#ifndef ZUMO_ROBOT_CAR_H
#define ZUMO_ROBOT_CAR_H

#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include "IMU/IMU.h"
#include "Motors/Motors.h"
#include "SensorArray/LineSensor/LineSensor.h"

class Car {
public:
    Motors motors;
    IMU imu;
    LineSensor lineSensor;

    static const uint16_t CAR_WIDTH = 98;
    static const uint16_t CAR_LENGTH = 98;
    static const uint16_t CAR_HEIGHT = 39;

    Car() = default;
    void setup();
    void update();
    void setup_line_follower();
    void follow_line(bool force_setup = false);
};
#endif