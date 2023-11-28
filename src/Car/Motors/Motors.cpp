#include "Motors.h"

template<typename T>
void swap(T& a, T& b);

void Motors::setup() {
    last_update = millis();
}

void Motors::update() {
    encoder.update();
    get_speed_and_position();
    get_drive_direction();
    update_movement();
    last_update = millis();
}

void Motors::update_movement() {
    switch (current_movement.action) {
        case Action::MOVE_TIME:
            move_time();
            break;

        case Action::MOVE_DISTANCE:
            move_distance();
            break;

        case Action::TURN_DEGREES:
            turn_degrees();
            break;

        case Action::TURN_WITH_RADIUS:
            turn_with_radius();
            break;

        default:
            stop_movement();
    }
}

void Motors::set_turn_angle(float new_turn_angle) {
    prev_turn_angle = turn_angle;
    turn_angle = new_turn_angle;
}

void Motors::set_movement(Movement new_movement) {
    current_movement = new_movement;
}

void Motors::get_speed_and_position() {
    uint32_t time_diff = millis() - last_update;

    left_speed = encoder.left_rotations * circumference;
    right_speed = encoder.right_rotations * circumference;

    left_distance += left_speed * time_diff / 1000.0;
    right_distance += right_speed * time_diff / 1000.0;

    distance = (left_distance + right_distance) / 2;
    speed = (left_speed + right_speed) / 2;
}

void Motors::get_drive_direction() {
    if (speed > 0) {
        direction = MovementDirection::FORWARD;
    } else if (speed < 0) {
        direction = MovementDirection::BACKWARD;
    } else {
        direction = MovementDirection::STATIONARY;
    }
}

void Motors::move(int16_t left_motor_speed, int16_t right_motor_speed, bool negate_speeds = false) {
    if (negate_speeds) {
        Zumo32U4Motors::setSpeeds(left_motor_speed, right_motor_speed);
    } else {
        Zumo32U4Motors::setSpeeds(-left_motor_speed, -right_motor_speed);
    }
}

void Motors::stop_movement() {
    if (current_movement.action != Action::NO_ACTION) {
        move(0, 0);

        current_movement.action = Action::NO_ACTION;
        current_movement.time_initialized = millis();
        current_movement.initialized_movement = true;
    }
}

void Motors::move_time() {
    if (!current_movement.initialized_movement) {
        move(max_speed, max_speed, !current_movement.move_forward);

        current_movement.time_initialized = millis();
        current_movement.initialized_movement = true;
    }

    if (current_movement.time_initialized < millis() - 1000) {
        stop_movement();
    }
}

void Motors::move_distance() {
    if (!current_movement.initialized_movement) {
        move(max_speed, max_speed, !current_movement.move_forward);

        current_movement.time_initialized = millis();
        current_movement.initialized_movement = true;
    }

    if (current_movement.move_distance >= distance) {
        stop_movement();
    }
}

void Motors::turn_degrees() {
    if (!current_movement.initialized_movement) {
        bool turn_right = current_movement.move_degrees > 0;
        move(-max_speed, max_speed, turn_right);

        current_movement.time_initialized = millis();
        current_movement.initialized_movement = true;
    }

    bool turn_right = current_movement.move_degrees > 0;
    float remaining_degrees = current_movement.move_degrees - turn_angle;
    float change_degrees = turn_angle - prev_turn_angle;

    const float KP = 10.0;
    const float KD = 1500.0;
    // int16_t speed_difference = KD * change_degrees - KP * remaining_degrees;
    // int16_t new_speed = max_speed - speed_difference;
    uint16_t new_speed = KP * remaining_degrees + KD * change_degrees;

    new_speed = constrain(new_speed, 0, 400);

    move(-new_speed, new_speed, turn_right);

    if (current_movement.move_degrees > 0) {
        if (turn_angle >= current_movement.move_degrees) {
            stop_movement();
        }
    } else {
        if (turn_angle <= current_movement.move_degrees) {
            stop_movement();
        }
    }
}

void Motors::turn_with_radius() {
    if (!current_movement.initialized_movement) {
        float radius_left = current_movement.move_radius - 49;
        float radius_right = current_movement.move_radius + 49;

        if (radius_right < radius_left) {
            swap(radius_left, radius_right);
        }

        float diff_r = radius_right ? 0 : radius_left / radius_right; // Safeguard against division by 0
        int16_t new_left_speed = ((int16_t)max_speed) * diff_r;
        int16_t new_right_speed = max_speed;

        bool move_right = signbit(current_movement.move_radius);
        if (move_right) {
            swap(new_left_speed, new_right_speed);
        }

        move(new_left_speed, new_right_speed, !current_movement.move_forward);

        current_movement.time_initialized = millis();
        current_movement.initialized_movement = true;
    }

    if (current_movement.move_radius < 0) {
        if (turn_angle <= current_movement.move_degrees) {
            stop_movement();
        }
    } else {
        if (turn_angle >= current_movement.move_degrees) {
            stop_movement();
        }
    }
}

template<typename T>
void swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}