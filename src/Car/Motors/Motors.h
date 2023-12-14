#ifndef ZUMO_ROBOT_MOTORS_H
#define ZUMO_ROBOT_MOTORS_H

#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>

#include "Encoders/Enoders.h"
#include "Movement/Movement.h"
#include "util/Vector/Vector.h"

enum MovementDirection {
    FORWARD,
    BACKWARD,
    STATIONARY
};

class Motors {
private:
    Encoder encoder;

    const float wheel_diameter = 39;
    const float circumference = PI * wheel_diameter;

    uint32_t last_update = 0;
    float left_distance = 0;
    float right_distance = 0;
    float left_speed = 0;
    float right_speed = 0;
    float turn_angle = 0;
    float prev_turn_angle = 0;

public:
    Movement current_movement = Movement_creator::no_move();
    MovementDirection direction = MovementDirection::STATIONARY;
    float distance = 0;
    Vector_2<float> distance_xy;
    float speed = 0;
    Vector_2<float> speed_xy;
    uint16_t max_speed = 400;
    float rotations_degrees = 0;

private:
    void get_speed_and_position();
    void get_drive_direction();
    void get_rotation();

    void update_movement();
    void move(int16_t left_speed, int16_t right_speed, bool flip_speeds);
    void stop_movement();
    void move_time();
    void move_distance();
    void turn_degrees();
    void turn_with_radius();

public:
    Motors() = default;
    void setup();
    void update();
    void set_turn_angle(float new_turn_angle);
    void set_movement(Movement new_movement);
};

#endif //ZUMO_ROBOT_MOTORS_H