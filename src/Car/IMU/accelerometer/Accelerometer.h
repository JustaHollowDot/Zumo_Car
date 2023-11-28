#ifndef ZUMO_ROBOT_POSITIONALDATA_H
#define ZUMO_ROBOT_POSITIONALDATA_H

#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include "util/AverageData/AverageData.h"
#include "util/Vector/Vector.h"

class Accelerometer {
public:
    Zumo32U4IMU *imu_a;

    Vector_3<int16_t> acceleration = Vector_3<int16_t>();
    Vector_3<int32_t> speed = Vector_3<int32_t>();
    Vector_3<int32_t> position = Vector_3<int32_t>();

    Vector_3<int16_t> acc_offset = Vector_3<int16_t>();
    uint32_t last_update;

    explicit Accelerometer(Zumo32U4IMU *imu);
    void setup();
    void reset();
    void update(Vector_3<double> angle);
    void get_acceleration(Vector_3<double> angle);
    void calculate_speed(Vector_3<double> angle);
    void calculate_position(Vector_3<double> angle);
};

#endif //ZUMO_ROBOT_POSITIONALDATA_H