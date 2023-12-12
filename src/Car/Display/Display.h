#ifndef ZUMO_ROBOT_DISPLAY_H
#define ZUMO_ROBOT_DISPLAY_H

#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include <Zumo32U4OLED.h>

template <typename T, typename U>
class Display {
private:
    Zumo32U4OLED display;

    static constexpr uint8_t messages_amount = 7;

public:
    String messages[3];

    String upper_menu_key[3];
    T upper_menu_values[3];
    String upper_menu_suffix[3];
    String lower_menu_key[4];
    U lower_menu_values[4];
    String lower_menu_suffix[4];

    Display() = default;

    void setup() {
        display.setLayout21x8();
    }

    void standard() {

    }

    void money() {

    }

    void battery() {

    }

    void update_messages() {
        display.gotoXY(0, 0);
        display.print("-MESSAGES----");
        display.print("");
        display.gotoXY(0, 2);
        display.print(messages[0]);
        display.gotoXY(0, 3);
        display.print("CF: ");
        display.print(messages[1]);
        display.gotoXY(0, 4);
        display.print("EMC: ");
        display.print(messages[2]);
    }

    void update_header() {
        display.gotoXY(0, 0);
        display.print("-MESSAGES----");
    }

    void add_message(const String& message) {
        display.noAutoDisplay();
        display.scrollDisplayUp();
        display.gotoXY(0, messages_amount);
        display.print(message);

        update_header();
        update_side_menu();
        display.display();
    }

    void update_side_menu() {
        for (int i = 0; i < 3; i++) {
            display_line(14, i, upper_menu_key[i], upper_menu_values[i], upper_menu_suffix[i]);
        }

        display.gotoXY(14, 3);
        display.print("|------");

        for (int i = 0; i < 4; i++) {
            display_line(14, i, lower_menu_key[i], lower_menu_values[i], lower_menu_suffix[i]);
        }
    }

    void display_line(uint8_t x, uint8_t y, const String& name, const T& value, const String& suffix) {
        display.gotoXY(x, y);
        display.print("|");
        display.print(name);
        display.print(": ");
        display.print(value);
        display.print(suffix);
    }

    void set_upper_menu_items(String new_names[3], T new_values[3], String new_suffixes[3]) {
        for (int i = 0; i < 3; i++) {
            upper_menu_key[i] = new_names[i];
            upper_menu_values[i] = new_values[i];
            upper_menu_suffix[i] = new_suffixes[i];
        }
    }

    void set_lower_menu_items(String new_names[4], U new_values[4], String new_suffixes[4]) {
        for (int i = 0; i < 4; i++) {
            lower_menu_key[i] = new_names[i];
            lower_menu_values[i] = new_values[i];
            lower_menu_suffix[i] = new_suffixes[i];
        }
    }
};

#endif //ZUMO_ROBOT_DISPLAY_H
