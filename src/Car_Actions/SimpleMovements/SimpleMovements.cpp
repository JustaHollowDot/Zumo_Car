#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include "util/AverageData/AverageData.h"
#include "util/PID_controller/PID_controller.h"
#include "util/Vector/Vector.h"
#include "Car/Car.h"
#include "SimpleMovements.h"

void SimpleMovements::drive_forward_and_back() {
    for (int i = 0; i < 2; i++) {
        car->motors.set_movement(Movement_creator::move_distance(car->motors.distance + 200));

        while (car->motors.current_movement.action != Action::NO_ACTION) {
            car->update();
        }

        car->imu.gyro.reset();
        car->motors.set_movement(Movement_creator::turn_degrees(car->imu.gyro.turn_angle.z + 180));

        while (car->motors.current_movement.action != Action::NO_ACTION) {
            car->update();
        }
    }
}

void SimpleMovements::drive_rectangle() {
    for (int i = 0; i < 4; i++) {
        car->motors.set_movement(Movement_creator::move_distance(car->motors.distance + 200));

        while (car->motors.current_movement.action != Action::NO_ACTION) {
            car->update();
        }

        car->imu.gyro.reset();
        car->motors.set_movement(Movement_creator::turn_degrees(car->imu.gyro.turn_angle.z + 90));

        while (car->motors.current_movement.action != Action::NO_ACTION) {
            car->update();
        }

        delay(100);
    }
}

void SimpleMovements::drive_circle() {
    for (int i = 1; i < 5; i++) {
        car->imu.gyro.reset();
        car->motors.set_movement(Movement_creator::turn_with_radius( 30 * i, car->imu.gyro.turn_angle.z + 360));

        while (car->motors.current_movement.action != Action::NO_ACTION) {
            car->update();
        }
    }
}

void SimpleMovements::drive_between_obstacles(int16_t radius, int8_t obstacles_amount) {
    for (int i = 0; i < obstacles_amount; i++) {
        if (i % 2 == 0) {
            car->imu.gyro.reset();
            car->motors.set_movement(Movement_creator::turn_with_radius(radius, car->imu.gyro.turn_angle.z + 180));

            while (car->motors.current_movement.action != Action::NO_ACTION) {
                car->update();
            }
        } else {
            car->imu.gyro.reset();
            car->motors.set_movement(Movement_creator::turn_with_radius(-radius, car->imu.gyro.turn_angle.z - 180));

            while (car->motors.current_movement.action != Action::NO_ACTION) {
                car->update();
            }
        }
    }
}
