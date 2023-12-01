#ifndef ZUMO_ROBOT_BATTERY_H
#define ZUMO_ROBOT_BATTERY_H

#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include <EEPROM.h>

class Battery {
public:
    static constexpr float battery_capacity = 100;
    static constexpr float ideal_battery_health = 100;
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
        battery_health = ideal_battery_health;
        current_battery_amount = battery_capacity;
        battery_percent = 100;
    }

    void deplete_battery(float used_amount) {
        current_battery_amount -= used_amount;
        current_battery_amount = constrain(current_battery_amount, 0, battery_capacity * battery_health / 100);
        battery_percent = current_battery_amount / battery_capacity;
    }

    void charge_battery(float added_amount) {
        current_battery_amount += added_amount * charge_factor;
        current_battery_amount = constrain(current_battery_amount, 0, battery_capacity * battery_health / 100);
        battery_percent = current_battery_amount / battery_capacity;

        added_battery_amount += added_amount;

        if (added_battery_amount >= battery_capacity) {
            charge_cycles++;
            added_battery_amount -= battery_capacity;
        }
    }

    void swap_battery() {
        battery_health = ideal_battery_health;
        current_battery_amount = battery_capacity;
    }

    void service() {
        battery_health += 10;
        battery_health = constrain(battery_health, 0, ideal_battery_health);
    }
};

#endif //ZUMO_ROBOT_BATTERY_H