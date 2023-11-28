#ifndef ZUMO_ROBOT_ENODERS_H
#define ZUMO_ROBOT_ENODERS_H

#include <Arduino.h>
#include <Zumo32U4.h>

class Encoder {
private:
    static constexpr float gear_ratio = 75.81;
    const int8_t counts_per_rotation = 12;

public:
    int16_t left_count = 0;
    int16_t right_count = 0;
    float left_rotations = 0;
    float right_rotations = 0;
    bool error_left = false;
    bool error_right = false;

    Encoder() = default;
    void update() {
        error_left = Zumo32U4Encoders::checkErrorLeft();
        error_right = Zumo32U4Encoders::checkErrorRight();

        left_count = Zumo32U4Encoders::getCountsAndResetLeft();
        right_count = Zumo32U4Encoders::getCountsAndResetRight();

        left_rotations = (float)left_count / (gear_ratio * counts_per_rotation);
        right_rotations = (float)right_count / (gear_ratio * counts_per_rotation);
    };
};

#endif //ZUMO_ROBOT_ENODERS_H