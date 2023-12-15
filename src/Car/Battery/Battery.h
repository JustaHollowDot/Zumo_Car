#ifndef ZUMO_ROBOT_BATTERY_H
#define ZUMO_ROBOT_BATTERY_H

#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include <EEPROM.h>

constexpr int32_t BATTERY_HEALTH_ADDRESS = 0;
constexpr int32_t CHARGE_CYCLE_ADDRESS = 0;

enum BatteryChargeState {
    CHARGING,
    DISCHARGING,
    EQUAL
};

enum EmergencyChargeResult {
    SUCCESS,
    TO_MUCH_CHARGE,
    EMERGENCY_CHARGE_UNAVAILABLE
};

enum BatteryCapacity {
    OK_BATTERY,
    LOW_BATTERY,
    VERY_LOW_BATTERY,
    CRITICAL_BATTERY
};

enum BatteryHealth {
    OK_HEALTH,
    BAD_HEALTH,
    VERY_BAD_HEALTH,
    CRITICAL_HEALTH
};

class Battery {
public:
    static constexpr float BATTERY_CAPACITY = 100;
    static constexpr float IDEAL_BATTERY_HEALTH = 100;
    BatteryChargeState charging_state = BatteryChargeState::EQUAL;
    float battery_health;
    float battery_percent;
    float current_battery_amount;
    float added_battery_amount = 0;
    bool emergency_charge_available = true;
    uint8_t charge_cycles = 0;
    uint8_t charge_factor = 1;

    Battery() = default;

    void setup();
    void update();
    void deplete_battery(float used_amount);
    void charge_battery(float added_amount);
    void drain_from_velocity(float current_speed);
    void swap_battery();
    void service();
    EmergencyChargeResult activate_emergency_mode();
    BatteryCapacity check_battery_amount();
    BatteryHealth check_battery_health();
};

#endif //ZUMO_ROBOT_BATTERY_H