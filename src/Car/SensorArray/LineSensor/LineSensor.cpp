#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>

#include "LineSensor.h"

void LineSensor::init() {
    lineSensors.initFiveSensors();
}

void LineSensor::reset() {
    lineSensors.resetCalibration();
    low_floor_value = -1;
}

void LineSensor::calibrate_sensors() {
    lineSensors.calibrate();

    for (auto x : line_sensor_values) {
        low_floor_value = x < low_floor_value ? x : low_floor_value;
    }

    uint16_t diff_floor_line = *lineSensors.calibratedMaximumOn - low_floor_value;

    high_floor_value = low_floor_value + diff_floor_line / 4;
    low_line_value = *lineSensors.calibratedMaximumOn - diff_floor_line / 4;
}

void LineSensor::update() {
    distance_from_center = lineSensors.readLine(line_sensor_values) - 2000;

    line_to_left = line_sensor_values[0] >= low_line_value;
    line_to_right = line_sensor_values[4] >= low_line_value;
    line_under_middle = line_sensor_values[2] >= low_line_value;
    is_at_intersection = (line_sensor_values[1] + line_sensor_values[2] + line_sensor_values[3]) / 3 > high_floor_value; // Might be set too low/high
}

bool LineSensor::check_if_initialized() {
    // calibratedMaximumOn points to array when initialized, it is 0 otherwise
    return *lineSensors.calibratedMaximumOn != 0;
}