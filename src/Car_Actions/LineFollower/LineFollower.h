#ifndef ZUMO_ROBOT_LINE_FOLLOWER_H
#define ZUMO_ROBOT_LINE_FOLLOWER_H

#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include "Car/LineSensor/LineSensor.h"
#include "util/AverageData/AverageData.h"
#include "util/PID_controller/PID_controller.h"
#include "util/Vector/Vector.h"
#include "Car/Car.h"

PID_controller pid_controller_line_follower = PID_controller(0.5, 8);

struct Edge {
    float length;
    uint8_t start_index;
    uint8_t end_index;
};

struct Node {
    Vector_2<float> coordinates;
    uint8_t edge_amount = 0;
    uint8_t edges_index[4] {};
    float edge_rotations[4] {};

    Node(Vector_2<float> coordinates, uint8_t edges[4], float edge_rotations[4]) {
        this->coordinates = coordinates;
        memcpy(&this->edges_index, &edges, sizeof(edges[0]) * 4);
        memcpy(&this->edge_rotations, &edge_rotations, sizeof(edge_rotations[0]) * 4);
    }

    Node() {
        this->coordinates = Vector_2<float>(0, 0);
    };
};

struct Waypoint {
    uint32_t distance_from_start_node;
    uint8_t start_node_index;
    uint8_t start_node_edge_index;
    uint8_t end_node_index;
    uint8_t end_node_edge_index;
};

constexpr uint8_t UNKNOWN_EDGE_INDEX = -1;
constexpr Edge unknown_edge {0, UNKNOWN_EDGE_INDEX, UNKNOWN_EDGE_INDEX};
constexpr uint8_t NO_EDGE_INDEX = -2;
constexpr Edge no_edge {0, NO_EDGE_INDEX, NO_EDGE_INDEX};

class LineFollower {
public:
    Car *car;

    uint8_t nodes_amount = 0;
    uint8_t edges_amount = 0;
    Node nodes[25];
    Edge edges[50];
    Waypoint charging_station;
    Waypoint sensor_node;
    uint8_t current_node_index;
    uint8_t current_path_index;
    float current_node_distance;

    int16_t last_error = 0;

    LineFollower(Car *car) {
        this->car = car;
    }

    void setup();
    void reset();
    void explore_maze();
    uint8_t get_last_unknown_edge_index();
    uint8_t get_next_to_last_node(uint8_t start_node_index, uint8_t end_node_index, uint8_t ignore_edge_index = NO_EDGE_INDEX, bool ignore_battery_station = false);
    void drive_to_node(uint8_t node_index);
    void drive_loop();
    void follow_path(const uint8_t *path, uint8_t path_length);
    void drive_loop_with_battery_charger();
    void drive_to_next_node();
    void follow_segment(float distance = 0);
    void drive_to_intersection_center();
    void add_node();
    void add_node_and_edges();
    void add_new_node();
    void add_edge_end(uint8_t start_node, uint8_t end_node, float length, uint8_t start_node_edge_index, uint8_t end_node_edge_index, float end_rotation);
    void add_unknown_edge(uint8_t current_node, float rotation);
};

#endif // ZUMO_ROBOT_LINE_FOLLOWER_H