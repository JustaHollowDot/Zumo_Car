#include "Movement.h"

Movement Movement_creator::no_move() {
    Movement temp;      // temp created as 'return Movement { ... }' does not work properly due to union field.
    temp.action = Action::NO_ACTION;
    temp.time_initialized = 0;
    temp.move_time = 0;
    temp.move_radius = 0;
    temp.move_forward = true;
    temp.initialized_movement = false;

    return temp;
}

Movement Movement_creator::move_time(int16_t time, bool drive_forward) {
    Movement temp;      // temp created as 'return Movement { ... }' does not work properly due to union field.
    temp.action = Action::MOVE_TIME;
    temp.time_initialized = 0;
    temp.move_time = time;
    temp.move_radius = 0;
    temp.move_forward = drive_forward;
    temp.initialized_movement = false;

    return temp;
}

Movement Movement_creator::move_distance(int16_t distance, bool drive_forward) {
    Movement temp;      // temp created as 'return Movement { ... }' does not work properly due to union field.
    temp.action = Action::MOVE_DISTANCE;
    temp.time_initialized = 0;
    temp.move_distance = distance;
    temp.move_radius = 0;
    temp.move_forward = drive_forward;
    temp.initialized_movement = false;

    return temp;
}

Movement Movement_creator::turn_degrees(int32_t degrees, bool drive_forward) {
    Movement temp;      // temp created as 'return Movement { ... }' does not work properly due to union field.
    temp.action = Action::TURN_DEGREES;
    temp.time_initialized = 0;
    temp.move_degrees = degrees;
    temp.move_radius = 0;
    temp.move_forward = drive_forward;
    temp.initialized_movement = false;

    return temp;
}

Movement Movement_creator::turn_with_radius(int16_t radius, int32_t degrees, bool drive_forward) {
    Movement temp;      // temp created as 'return Movement { ... }' does not work properly due to union field.
    temp.action = Action::TURN_WITH_RADIUS;
    temp.time_initialized = 0;
    temp.move_degrees = degrees;
    temp.move_radius = radius;
    temp.move_forward = drive_forward;
    temp.initialized_movement = false;

    return temp;
}