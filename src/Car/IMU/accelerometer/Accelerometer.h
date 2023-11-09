#ifndef ZUMO_ROBOT_POSITIONALDATA_H
#define ZUMO_ROBOT_POSITIONALDATA_H

#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include "util/AverageData/AverageData.h"
#include "util/Vector/Vector.h"

class Accelerometer {
public:
    Zumo32U4IMU imu_a;

    Vector_3<AverageData<int32_t>> acceleration = Vector_3<AverageData<int32_t>>();
    Vector_3<int32_t> speed = Vector_3<int32_t>();
    Vector_3<int32_t> position = Vector_3<int32_t>();

    int32_t acc_offset[3] = {0};
    uint32_t last_update;

    explicit Accelerometer(Zumo32U4IMU imu);
    void reset();
    void update();
    void get_acceleration();
    void calculate_speed();
    void calculate_position();
};

#endif //ZUMO_ROBOT_POSITIONALDATA_H