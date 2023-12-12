#include "Motors.h"
#include "util/PID_controller/PID_controller.h"

PID_controller pid_controller = PID_controller(50, 50, 1);

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
    get_rotation();
    last_update = millis();
}

void Motors::update_movement() {
    switch (current_movement.action) {
        case Action::MOVE:
            move(current_movement.left_speed, current_movement.right_speed, current_movement.move_forward);
            break;

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

    left_speed = encoder.left_rotations * circumference * time_diff / 1000.0;
    right_speed = encoder.right_rotations * circumference * time_diff / 1000.0;

    left_distance += encoder.left_rotations * circumference;
    right_distance += encoder.right_rotations * circumference;

    float added_distance = (left_distance + right_distance) / 2.0;
    distance += added_distance;
    distance_xy.x += added_distance * cos(rotations_degrees);
    distance_xy.y += added_distance * sin(rotations_degrees);

    float new_speed = (left_speed + right_speed) / 2.0;
    speed = new_speed;
    speed_xy.x = new_speed * cos(rotations_degrees);
    speed_xy.y = new_speed * sin(rotations_degrees);
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

void Motors::get_rotation() {
    constexpr uint8_t car_width = 98;
    constexpr float circle_circumference = PI * car_width * 2; // Get circumference from circle where only one track moves.

    float distance_difference = left_distance - right_distance;

    rotations_degrees = distance_difference / circle_circumference * 360;
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
        pid_controller.reset();
        move(max_speed, max_speed, !current_movement.move_forward);

        current_movement.time_initialized = millis();
        current_movement.initialized_movement = true;
    }

    int16_t new_speed = max_speed / 2 + pid_controller.get_pid_value(millis() - current_movement.move_time);
    new_speed = constrain(new_speed, 0, 400);

    move(new_speed, new_speed, !current_movement.move_forward);

    if (current_movement.time_initialized < millis() - current_movement.move_time) {
        stop_movement();
    }
}

void Motors::move_distance() {
    if (!current_movement.initialized_movement) {
        pid_controller.reset();

        current_movement.time_initialized = millis();
        current_movement.initialized_movement = true;
    }

    int16_t new_speed = max_speed / 2 + pid_controller.get_pid_value(distance);
    new_speed = constrain(new_speed, 0, 400);

    move(new_speed, new_speed, current_movement.move_forward);

    if (current_movement.move_distance >= distance) {
        stop_movement();
    }
}


void Motors::turn_degrees() {
    if (!current_movement.initialized_movement) {
        pid_controller.reset();

        current_movement.time_initialized = millis();
        current_movement.initialized_movement = true;
    }

    bool turn_right = current_movement.move_degrees > 0;
    float remaining_degrees = current_movement.move_degrees - turn_angle;

    int16_t new_speed = pid_controller.get_pid_value(remaining_degrees);
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
        int16_t new_left_speed = (int16_t)((float)max_speed * diff_r);
        int16_t new_right_speed = max_speed;

        bool move_right = signbit(current_movement.move_radius);
        if (move_right) {
            swap(new_left_speed, new_right_speed);
        }

        move(new_left_speed, new_right_speed, !current_movement.move_forward);

        current_movement.time_initialized = millis();
        current_movement.initialized_movement = true;
    }

    int16_t new_speed = max_speed / 2 + pid_controller.get_pid_value(distance);
    new_speed = constrain(new_speed, 0, 400);

    move(new_speed, new_speed, current_movement.move_forward);

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