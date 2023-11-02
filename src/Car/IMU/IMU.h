#ifndef ZUMO_ROBOT_IMU_H
#define ZUMO_ROBOT_IMU_H

#include <Arduino.h>


class IMU {
public:
    IMU() = default;
    void setup();
    void update();


};

#endif //ZUMO_ROBOT_IMU_H
