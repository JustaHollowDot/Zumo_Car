#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>

#include "LineSensor.h"

void LineSensor::init() {
    lineSensors.initFiveSensors();
}

void LineSensor::reset() {
    lineSensors.resetCalibration();
}

void LineSensor::calibrate_sensors() {
    lineSensors.calibrate();
}

void LineSensor::update() {
    distance_from_center = lineSensors.readLine(line_sensor_values) - 2000;
}

bool LineSensor::check_if_initialized() {
    // calibratedMaximumOn points to array when initialized, it is 0 otherwise
    return *lineSensors.calibratedMaximumOn != 0;
}

