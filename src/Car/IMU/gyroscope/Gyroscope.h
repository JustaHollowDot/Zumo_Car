#ifndef TurnSensor_h
#define TurnSensor_h

#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include "util/Vector/Vector.h"

class Gyroscope {
public:
    Zumo32U4IMU *imu_g;
    static constexpr int32_t turn_45 = 0x20000000;
    static constexpr int32_t turn_90 = 0x40000000;
    static constexpr int32_t turn_1 = (0x20000000 + 22) / 45;
    static constexpr float radian_1 = (float) turn_1 * PI / 180;

    static constexpr float degrees_per_second = 0.07;
    static constexpr float micro = 1e-6;
    static constexpr float gyro_digits_to_degrees_per_second = degrees_per_second * micro * turn_1;

    Vector_3<float> turn_angle;
    Vector_3<float> turn_rate;
    Vector_3<float> gyro_offset;
    uint32_t last_update = 0;

    explicit Gyroscope(Zumo32U4IMU *imu);
    void setup();
    void reset();
    void update();
};

#endif
