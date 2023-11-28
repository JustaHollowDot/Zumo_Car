#ifndef ZUMO_ROBOT_MOVEMENT_H
#define ZUMO_ROBOT_MOVEMENT_H

#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>

enum class Action {
    MOVE_TIME,
    MOVE_DISTANCE,
    TURN_DEGREES,
    TURN_WITH_RADIUS,
    FOLLOW_LINE,
    NO_ACTION
};

struct Movement {
    Action action;
    uint32_t time_initialized;

    // Combines the three variables into one memory location as at most one is used at a time
    union {
        int32_t move_time;
        int32_t move_distance;
        int32_t move_degrees;
    };

    int16_t move_radius;
    bool move_forward;
    bool initialized_movement;

};

class Movement_creator {
public:
    static Movement no_move();
    static Movement move_time(int16_t time, bool drive_forward = true);
    static Movement move_distance(int16_t distance, bool drive_forward = true);
    static Movement turn_degrees(int32_t degrees, bool drive_forward = true);
    static Movement turn_with_radius(int16_t radius, int32_t degrees, bool drive_forward = true);
};

#endif //ZUMO_ROBOT_MOVEMENT_H