#ifndef ZUMO_ROBOT_SIMPLEMOVEMENTS_H
#define ZUMO_ROBOT_SIMPLEMOVEMENTS_H

#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include "util/AverageData/AverageData.h"
#include "util/PID_controller/PID_controller.h"
#include "util/Vector/Vector.h"
#include "Car/Car.h"

class SimpleMovements {
public:
    Car *car;

    SimpleMovements(Car *car) {
        this->car = car;
    }

    void drive_forward_and_back();
    void drive_rectangle();
    void drive_circle();
    void drive_between_obstacles(int16_t radius, int8_t obstacles_amount);
};

#endif //ZUMO_ROBOT_SIMPLEMOVEMENTS_H
