#ifndef ZUMO_ROBOT_CAR_H
#define ZUMO_ROBOT_CAR_H

#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include "IMU/IMU.h"
#include "Motors/Motors.h"
#include "SensorArray/LineSensor/LineSensor.h"
#include "Display/Display.h"
#include "Battery/Battery.h"

class Car {
public:
    Motors motors;
    IMU imu;
    LineSensor lineSensor;
    Display<String, String> display;
    Battery battery;

    static constexpr uint16_t CAR_WIDTH = 98;
    static constexpr uint16_t CAR_LENGTH = 98;
    static constexpr uint16_t CAR_HEIGHT = 39;

    Car() = default;
    void setup();
    void update();

    void display_standard();

    void setup_line_follower();
    void follow_line();
};
#endif