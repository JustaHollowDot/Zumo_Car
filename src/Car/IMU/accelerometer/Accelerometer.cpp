#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include "Accelerometer.h"

Accelerometer::Accelerometer(Zumo32U4IMU *imu) {
    this->imu_a = imu;
}

void Accelerometer::setup() {
    uint16_t acc_update_counter = 0;
    Vector_3<int32_t> acc_totals = Vector_3<int32_t>();

    uint16_t update_amount = 256;
    for ( ; ; ) {
        if (imu_a->accDataReady()) {
            imu_a->readAcc();
            acc_totals.x += imu_a->a.x;
            acc_totals.y += imu_a->a.y;
            acc_totals.z += imu_a->a.z;

            acc_update_counter++;
        }

        if (acc_update_counter >= update_amount) {
            break;
        }
    }

    acc_offset.x = acc_totals.x / acc_update_counter;
    acc_offset.y = acc_totals.y / acc_update_counter;
    acc_offset.z = acc_totals.z / acc_update_counter;

    last_update = micros();
}

void Accelerometer::reset() {
    last_update = micros();

    acceleration.reset();
    speed.reset();
    position.reset();
}

void Accelerometer::update(Vector_3<double> angle) {
    get_acceleration(angle);
    calculate_speed(angle);
    calculate_position(angle);
}

void Accelerometer::get_acceleration(Vector_3<double> angle) {
    imu_a->readAcc();

    Vector_3<int16_t> temp_acc = {imu_a->a.x, imu_a->a.y, imu_a->a.z};
    temp_acc -= acc_offset;

    acceleration = temp_acc;
}

void Accelerometer::calculate_speed(Vector_3<double> angle) {
    auto temp = acceleration;
    // temp *= ((float)(micros() - last_update)) / 1e6;
    speed -= temp;
}

void Accelerometer::calculate_position(Vector_3<double> angle) {
    auto temp = speed;
    // temp *= (float)(micros() - last_update) / 1e6;
    position += temp;
}

