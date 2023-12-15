#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include <Zumo32U4OLED.h>
#include "Display.h"

void Display::setup() {
    display.setLayout21x8();
}

void Display::standard() {

}

void Display::money() {

}

void Display::battery() {

}

void Display::update_messages() {
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

void Display::update_header() {
    display.gotoXY(0, 0);
    display.print("-MESSAGES----");
}

void Display::add_message(const String& message) {
    display.noAutoDisplay();
    display.scrollDisplayUp();
    display.gotoXY(0, messages_amount);
    display.print(message);

    update_header();
    update_side_menu();
    display.display();
}

void Display::update_side_menu() {
    for (int i = 0; i < 3; i++) {
        display_line(14, i, upper_menu_key[i], upper_menu_values[i], upper_menu_suffix[i]);
    }

    display.gotoXY(14, 3);
    display.print("|------");

    for (int i = 0; i < 4; i++) {
        display_line(14, i, lower_menu_key[i], lower_menu_values[i], lower_menu_suffix[i]);
    }
}

void Display::display_line(uint8_t x, uint8_t y, const String& name, const int32_t& value, const String& suffix) {
    display.gotoXY(x, y);
    display.print("|");
    display.print(name);
    display.print(": ");
    display.print(value);
    display.print(suffix);
}

void Display::set_upper_menu_items(String new_names[3], int32_t new_values[3], String new_suffixes[3]) {
    for (int i = 0; i < 3; i++) {
        upper_menu_key[i] = new_names[i];
        upper_menu_values[i] = new_values[i];
        upper_menu_suffix[i] = new_suffixes[i];
    }
}

void Display::set_lower_menu_items(String new_names[4], int32_t new_values[4], String new_suffixes[4]) {
    for (int i = 0; i < 4; i++) {
        lower_menu_key[i] = new_names[i];
        lower_menu_values[i] = new_values[i];
        lower_menu_suffix[i] = new_suffixes[i];
    }
}
