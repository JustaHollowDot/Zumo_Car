#ifndef ZUMO_ROBOT_DISPLAY_H
#define ZUMO_ROBOT_DISPLAY_H

#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include <Zumo32U4OLED.h>

class Display {
private:
    Zumo32U4OLED display;

    static constexpr uint8_t messages_amount = 7;

public:
    String messages[3];

    String upper_menu_key[3];
    int32_t upper_menu_values[3];
    String upper_menu_suffix[3];
    String lower_menu_key[4];
    int32_t lower_menu_values[4];
    String lower_menu_suffix[4];

    Display() = default;

    void setup();
    void standard();
    void money();
    void battery();
    void update_messages();
    void update_header();
    void add_message(const String& message);
    void update_side_menu();
    void display_line(uint8_t x, uint8_t y, const String& name, const int32_t& value, const String& suffix);
    void set_upper_menu_items(String new_names[3], int32_t new_values[3], String new_suffixes[3]);
    void set_lower_menu_items(String new_names[4], int32_t new_values[4], String new_suffixes[4]);
};

#endif //ZUMO_ROBOT_DISPLAY_H
