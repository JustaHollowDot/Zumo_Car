#ifndef ZUMO_ROBOT_LINESENSOR_H
#define ZUMO_ROBOT_LINESENSOR_H

#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>

constexpr int8_t NUM_SENSORS = 5;

class LineSensor {
private:
    Zumo32U4LineSensors lineSensors;
    uint16_t low_floor_value = -1; // Set to the highest possible integer to ensure the lowest sensor-value to be retained.
    uint16_t high_floor_value;
    uint16_t low_line_value;

public:
    uint16_t line_sensor_values[NUM_SENSORS] = {0};
    int16_t distance_from_center = 0;
    int16_t last_distance_from_center = 0;
    bool line_to_left = false;
    bool line_to_right = false;
    bool line_under_middle = false;
    bool is_at_intersection = false;

    LineSensor() = default;

    void init();
    void reset();
    void calibrate_sensors();
    void update();
    bool check_if_initialized();
};

#endif //ZUMO_ROBOT_LINESENSOR_H
