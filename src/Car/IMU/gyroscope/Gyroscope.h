#ifndef TurnSensor_h
#define TurnSensor_h

#include <Arduino.h>

class TurnSensor {
public:
    const int32_t turn_45 = 0x20000000;
    const int32_t turn_90 = 0x40000000;
    const int32_t turn_1 = (0x20000000 + 22) / 45;

    int64_t turn_angle = 0;
    int64_t turn_rate;
    int64_t gyro_offset = 0;
    uint32_t last_update = 0;

    TurnSensor() = default;

    void reset();
    void update();
    void setup();
};

#endif
