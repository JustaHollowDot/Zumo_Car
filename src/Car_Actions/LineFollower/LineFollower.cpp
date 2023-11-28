/*

#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>

#include "Car/Car.h"

#define NUM_SENSORS 5 // Number of line sensors in use

// Initialising Zumo32U4 Components
Zumo32U4LineSensors lineSensors;     // Line Sensors
Zumo32U4Buzzer buzzer;               // Buzzer
Zumo32U4Motors motors;               // Motors
Zumo32U4OLED display;                // Display
Zumo32U4ButtonA buttonA;             // A button


const uint16_t max_speed = 300;               // Set max speed
unsigned int line_sensor_values[NUM_SENSORS]; // Array for storing line sensor values
int16_t last_error = 0;                       // Last error position

void calibrate_sensors() {
    display.clear();
    delay(1000);
    for(uint16_t i = 0; i < 100; i++) {
        motors.setSpeeds(-200, 200);
        lineSensors.calibrate();
    }
    motors.setSpeeds(0, 0);
}

void follow_line() {
    int16_t position = lineSensors.readLine(line_sensor_values);        // Reads the position
    int16_t error = position - 2000;                                                // Adjust the ideal position to 0 instead of 2000
    int16_t speed_difference = error / 3 + 8 * (error - last_error);                // Speed difference. Default: 3 + 4
    last_error = error;

    int16_t left_speed = (int16_t)max_speed + speed_difference;
    int16_t right_speed = (int16_t)max_speed - speed_difference;

    left_speed = constrain(left_speed, 0, (int16_t)max_speed);
    right_speed = constrain(right_speed, 0, (int16_t)max_speed);

    motors.setSpeeds(left_speed, right_speed);
}

void setup() {
    lineSensors.initFiveSensors();

    display.clear();
    display.print(F("press A"));
    display.gotoXY(0,1);
    display.print(F("to cal"));
    buttonA.waitForButton();
    buzzer.play("c150");
    calibrate_sensors();
    display.clear();
}

void loop() {
    follow_line();
}
 */