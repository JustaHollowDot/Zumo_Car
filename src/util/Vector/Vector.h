#ifndef ZUMO_ROBOT_VECTOR_H
#define ZUMO_ROBOT_VECTOR_H

#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include "util/AverageData/AverageData.h"

template< typename T>
struct Vector_3 {
    T x;
    T y;
    T z;

    void reset() {
        x = decltype(x)(0); // reinitialize the variable x to the value 0 with the same type
        y = decltype(y)(0); // int x = (int) 0
        z = decltype(z)(0); // AverageData<T> x = (AverageData<T>) 0
    }

    template<typename U>
    void operator+=(Vector_3<U> rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
    }

    template<typename U>
    void operator-=(Vector_3<U> rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
    }
};

#endif //ZUMO_ROBOT_VECTOR_H