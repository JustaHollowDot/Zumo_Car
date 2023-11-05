#ifndef ZUMO_ROBOT_POSITIONALDATA_H
#define ZUMO_ROBOT_POSITIONALDATA_H

#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>

struct AverageData {
    int32_t dataset[10] = {0};
    int32_t mean_value = 0;
    int32_t idx = 0;
    int32_t sum = 0;
    bool first_run = true;

    void add_data(int32_t number) {
        if (first_run && idx == 9) {
            first_run = false;
        }

        sum -= dataset[idx];
        sum += number;
        dataset[idx] = number;

        idx++;
        idx %= 10;

        if (first_run) {
            mean_value = sum / idx;
        } else {
            mean_value = sum / 10;
        }
    }

    void reset() {
        for (int32_t & i : dataset) {
            i = 0;
        }

        idx = 0;
        sum = 0;
        first_run = true;
    }
};

template< typename T>
struct Vector_3 {
    T x;
    T y;
    T z;

    void reset() {
        x = 0;
        y = 0;
        z = 0;
    }

    Vector_3<T> operator+=(Vector_3<T> rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
    }
};

template<> void Vector_3<AverageData>::reset() {
    x.reset();
    y.reset();
    z.reset();
}


class Accelerometer {
public:
    Zumo32U4IMU imu_a;

    Vector_3<AverageData> acceleration = Vector_3<AverageData>();
    Vector_3<int32_t> speed = Vector_3<int32_t>();
    Vector_3<int32_t> position = Vector_3<int32_t>();

    int32_t acc_offset[3] = {0};
    uint32_t last_update;

    explicit Accelerometer(Zumo32U4IMU imu);
    void reset();
    void update();
    void get_acceleration();
    void calculate_speed();
    void calculate_position();
};

#endif //ZUMO_ROBOT_POSITIONALDATA_H