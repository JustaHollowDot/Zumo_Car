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

/*
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


template<class T>
class AverageData {
private:
    T val;
    T dataset[10] = {0};
    T sum = 0;
    uint8_t idx = 0;
    bool first_run = true;

public:

};
*/

#endif //ZUMO_ROBOT_AVERAGEDATA_H
