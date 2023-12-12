#ifndef ZUMO_ROBOT_PID_CONTROLLER_H
#define ZUMO_ROBOT_PID_CONTROLLER_H

#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include "util/AverageData/AverageData.h"

class PID_controller {
public:
    float KP;
    float KD;
    float KI;

    float last_value = 0;
    float average_difference = 0;
    float average_integral = 0;

    /*
    AverageData<float, 5> average_difference;
    AverageData<float, 5> average_integral;
     */

    explicit PID_controller(float kp, float kd = 0, float ki = 0) {
        KP = kp;
        KD = kd;
        KI = ki;
    }

    void reset() {
        average_difference = 0;
        average_integral = 0;

        /*
        average_difference.reset();
        average_integral.reset();
         */
    }

    float get_pid_value(float value) {
        average_difference = value - last_value;
        average_integral = value + last_value;

        float kp_value = KP * value;
        float kd_value = KD * average_difference;
        float ki_value = KI * average_integral;

        last_value = value;

        return kp_value + kd_value + ki_value;
    }
};

#endif //ZUMO_ROBOT_PID_CONTROLLER_H