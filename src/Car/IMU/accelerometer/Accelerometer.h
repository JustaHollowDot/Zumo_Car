#ifndef ZUMO_ROBOT_POSITIONALDATA_H
#define ZUMO_ROBOT_POSITIONALDATA_H

#include <Arduino.h>

struct AverageData {
    int64_t dataset[10] = {0};
    int64_t mean_value = 0;
    int64_t idx = 0;
    int64_t sum = 0;
    bool first_run = true;

    void add_data(int64_t number) {
        if (first_run && idx == 9) {
            first_run = false;
        }

        sum += number;
        sum -= dataset[idx];

        idx++;
        idx %= 10;
        mean_value = first_run ? sum / (idx + 1) : sum / 10;
    }

    /*
    long long get_mean() {
        return first_run ? sum / (idx + 1) : sum / 10;
    }
    */
};

template <typename T>
struct Vector_3 {
    T x = 0;
    T y = 0;
    T z = 0;
};

class PositionalData {
public:
    Vector_3<int64_t> position = Vector_3<int64_t>();
    Vector_3<int64_t> speed = Vector_3<int64_t>();
    Vector_3<int64_t> acceleration = Vector_3<int64_t>();

    PositionalData() = default;
};

#endif //ZUMO_ROBOT_POSITIONALDATA_H
