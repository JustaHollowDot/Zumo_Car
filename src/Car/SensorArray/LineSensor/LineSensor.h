#ifndef ZUMO_ROBOT_LINESENSOR_H
#define ZUMO_ROBOT_LINESENSOR_H

#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>

constexpr int8_t NUM_SENSORS = 5;

class LineSensor {
private:
    Zumo32U4LineSensors lineSensors;

public:
    uint16_t line_sensor_values[NUM_SENSORS];
    int16_t distance_from_center;
    int16_t last_distance_from_center;

    LineSensor() = default;

    void init();
    void reset();
    void calibrate_sensors();
    void update();
    bool check_if_initialized();
};

#endif //ZUMO_ROBOT_LINESENSOR_H
