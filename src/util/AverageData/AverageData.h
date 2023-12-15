#ifndef ZUMO_ROBOT_AVERAGEDATA_H
#define ZUMO_ROBOT_AVERAGEDATA_H

#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>

template<class T, const uint16_t dataset_amount = 10>
class AverageData {
private:
    T val;
    T dataset[dataset_amount] = {};
    T sum = 0;
    uint16_t idx = 0;
    bool first_run = true;

    inline void set_val() {
        if (first_run) {
            val = sum / idx;
        } else {
            val = sum / 10;
        }
    }

public:
    AverageData(T val = 0) : val(val) {
        this->val = val;
    }

    operator T &() { return val; }

    template<typename U>
    AverageData& operator=(U rhs) {
        if (first_run && idx == dataset_amount - 1) {
            first_run = false;
        }

        sum -= dataset[idx];
        sum += rhs;
        dataset[idx] = rhs;


        idx++;
        idx %= dataset_amount;

        set_val();

        return *this;
    }

    template<typename U>
    T operator+(U rhs) {
        return val + rhs;
    }

    template<typename U>
    T operator-(U rhs) {
        return val - rhs;
    }

    template<typename U>
    T operator*(U rhs) {
        return val * rhs;
    }

    template<typename U>
    T operator/(U rhs) {
        return val / rhs;
    }

    template<typename U>
    void operator+=(U rhs) {
        if (first_run && idx == dataset_amount - 1) {
            first_run = false;
        }

        T tmp = dataset[idx - 1] + rhs;

        sum -= dataset[idx];
        sum += tmp;
        dataset[idx] = tmp;

        idx++;
        idx %= dataset_amount;

        set_val();
    }

    template<typename U>
    void operator==(U rhs) {
        return val == rhs;
    }

    template<typename U>
    void operator<(U rhs) {
        return val < rhs;
    }

    void reset() {
        first_run = true;
        sum = 0;
        idx = 0;
    }
};

#endif //ZUMO_ROBOT_AVERAGEDATA_H
