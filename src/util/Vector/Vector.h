#ifndef ZUMO_ROBOT_VECTOR_H
#define ZUMO_ROBOT_VECTOR_H

#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include "util/AverageData/AverageData.h"

template<typename T>
struct Vector_2 {
    T x;
    T y;

    Vector_2(T x = 0, T y = 0, T z = 0) {
        this->x = x;
        this->y = y;
    }

    void reset() {
        x = decltype(x)(0); // reinitialize the variable x to the value 0 with the same type
        y = decltype(y)(0); // int x = (int) 0
    }

    template<typename U>
    void operator+=(Vector_2<U> rhs) {
        x += rhs.x;
        y += rhs.y;
    }

    template<typename U>
    void operator-=(Vector_2<U> rhs) {
        x -= rhs.x;
        y -= rhs.y;
    }

    template<typename U>
    Vector_2<T> operator*=(Vector_2<U> rhs) {
        x *= rhs.x;
        y *= rhs.y;

        return *this;
    }

    template<typename U>
    Vector_2<T> operator+=(U rhs) {
        x += rhs;
        y += rhs;

        return *this;
    }

    template<typename U>
    Vector_2<T> operator-=(U rhs) {
        x -= rhs;
        y -= rhs;

        return *this;
    }

    template<typename U>
    Vector_2<T> operator*=(U rhs) {
        x *= rhs;
        y *= rhs;

        return *this;
    }

};

template< typename T>
struct Vector_3 {
    T x;
    T y;
    T z;

    Vector_3(T x = 0, T y = 0, T z = 0) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

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

    template<typename U>
    Vector_3<T> operator*=(Vector_3<U> rhs) {
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;

        return *this;
    }

    template<typename U>
    Vector_3<T> operator+=(U rhs) {
        x += rhs;
        y += rhs;
        z += rhs;

        return *this;
    }

    template<typename U>
    Vector_3<T> operator-=(U rhs) {
        x -= rhs;
        y -= rhs;
        z -= rhs;

        return *this;
    }

    template<typename U>
    Vector_3<T> operator*=(U rhs) {
        x *= rhs;
        y *= rhs;
        z *= rhs;

        return *this;
    }
};

#endif //ZUMO_ROBOT_VECTOR_H