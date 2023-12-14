#ifndef ZUMO_ROBOT_BATTERY_H
#define ZUMO_ROBOT_BATTERY_H

#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include <EEPROM.h>

constexpr int32_t BATTERY_HEALTH_ADDRESS = 0;
constexpr int32_t CHARGE_CYCLE_ADDRESS = 0;


class Battery {
public:
    static constexpr float BATTERY_CAPACITY = 100;
    static constexpr float IDEAL_BATTERY_HEALTH = 100;
    float battery_health;
    float battery_percent;
    float current_battery_amount;
    float added_battery_amount = 0;
    float battery_drain;
    bool emergency_charge_available = true;
    uint8_t charge_cycles = 0;
    uint8_t charge_factor = 1;

    Battery() = default;

    void setup() {
        battery_health = IDEAL_BATTERY_HEALTH;
        current_battery_amount = BATTERY_CAPACITY;
        battery_percent = 100;
    }

    void deplete_battery(float used_amount) {
        current_battery_amount -= used_amount;
        current_battery_amount = constrain(current_battery_amount, 0, BATTERY_CAPACITY * battery_health / 100);
        battery_percent = current_battery_amount / BATTERY_CAPACITY;
    }

    void charge_battery(float added_amount) {
        current_battery_amount += added_amount * charge_factor;
        current_battery_amount = constrain(current_battery_amount, 0, BATTERY_CAPACITY * battery_health / 100);
        battery_percent = current_battery_amount / BATTERY_CAPACITY;

        added_battery_amount += added_amount;

        if (added_battery_amount >= BATTERY_CAPACITY) {
            charge_cycles++;
            added_battery_amount -= BATTERY_CAPACITY;

            battery_health -= charge_cycles;
            EEPROM.write(BATTERY_HEALTH_ADDRESS, battery_health);
            EEPROM.write(CHARGE_CYCLE_ADDRESS, charge_cycles);
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
};

#endif //ZUMO_ROBOT_BATTERY_H