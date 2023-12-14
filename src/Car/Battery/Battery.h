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

    void setup() {
        battery_health = IDEAL_BATTERY_HEALTH;
        current_battery_amount = BATTERY_CAPACITY;
        battery_percent = 100;
    }

    void update() {
        if (battery_percent >= 20) {
            charge_factor = 1;
        }
    }

    void deplete_battery(float used_amount) {
        current_battery_amount -= used_amount;
        current_battery_amount = constrain(current_battery_amount, 0, BATTERY_CAPACITY * battery_health / 100);
        battery_percent = current_battery_amount / BATTERY_CAPACITY * battery_health / 100;

        charging_state = BatteryChargeState::DISCHARGING;
    }

    void charge_battery(float added_amount) {
        current_battery_amount += added_amount * charge_factor;
        current_battery_amount = constrain(current_battery_amount, 0, BATTERY_CAPACITY * battery_health / 100);
        battery_percent = current_battery_amount / BATTERY_CAPACITY * battery_health / 100;

        added_battery_amount += added_amount;

        if (added_battery_amount >= BATTERY_CAPACITY) {
            charge_cycles++;
            added_battery_amount -= BATTERY_CAPACITY;

            battery_health -= charge_cycles;
            battery_health = constrain(battery_health, 0, IDEAL_BATTERY_HEALTH);

            EEPROM.write(BATTERY_HEALTH_ADDRESS, battery_health);
            EEPROM.write(CHARGE_CYCLE_ADDRESS, charge_cycles);
        }

        charging_state = BatteryChargeState::CHARGING;
    }

    void drain_from_velocity(float current_speed) {
        float battery_drain_amount = current_speed;

        if (battery_drain_amount < 0) {
            charge_battery(added_battery_amount);
        } else if (battery_drain_amount > 0) {
            deplete_battery(battery_drain_amount);
        } else {
            charging_state = BatteryChargeState::EQUAL;
        }
    }

    void swap_battery() {
        battery_health = IDEAL_BATTERY_HEALTH;
        current_battery_amount = BATTERY_CAPACITY;
    }

    void service() {
        battery_health += 10;
        battery_health = constrain(battery_health, 0, IDEAL_BATTERY_HEALTH);
    }

    EmergencyChargeResult activate_emergency_mode() {
        if (emergency_charge_available) {
            if (battery_percent < 20) {
                emergency_charge_available = false;

                charge_factor = 10;

                return EmergencyChargeResult::SUCCESS;
            } else {
                return EmergencyChargeResult::TO_MUCH_CHARGE;
            }
        } else {
            return EmergencyChargeResult::EMERGENCY_CHARGE_UNAVAILABLE;
        }
    }

    BatteryCapacity check_battery_amount() {
        if (battery_percent < 5) {
            return BatteryCapacity::CRITICAL_BATTERY;
        } else if (battery_percent < 10) {
            return BatteryCapacity::VERY_LOW_BATTERY;
        } else if (battery_percent < 20) {
            return BatteryCapacity::LOW_BATTERY;
        } else {
            return BatteryCapacity::OK_BATTERY;
        }
    }

    BatteryHealth check_battery_health() {
        if (battery_percent < 5) {
            return BatteryHealth::CRITICAL_HEALTH;
        } else if (battery_percent < 10) {
            return BatteryHealth::VERY_BAD_HEALTH;
        } else if (battery_percent < 20) {
            return BatteryHealth::BAD_HEALTH;
        } else {
            return BatteryHealth::OK_HEALTH;
        }
    }
};

#endif //ZUMO_ROBOT_BATTERY_H