#ifndef ZUMO_ROBOT_LINE_FOLLOWER_H
#define ZUMO_ROBOT_LINE_FOLLOWER_H

#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include "Car/SensorArray/LineSensor/LineSensor.h"
#include "util/AverageData/AverageData.h"
#include "util/PID_controller/PID_controller.h"
#include "util/Vector/Vector.h"
#include "Car/Car.h"

struct Edge {
    uint32_t length;
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
};

struct Waypoint {
    uint32_t distance_from_start_node;
    uint8_t start_node_index;
    uint8_t start_node_edge_index;
    uint8_t end_node_index;
    uint8_t end_node_edge_index;
};

constexpr uint8_t UNKNOWN_EDGE_INDEX = 100;
constexpr Edge unknown_edge {0, 100, 100};
constexpr uint8_t NO_EDGE_INDEX = 101;
constexpr Edge no_edge {0, 101, 101};

class LineFollower {
public:
    Car *car;

    uint8_t nodes = 0;
    Node path[50];
    Edge edges[100];
    Waypoint charging_station;
    Waypoint sensor_node;

    void setup() {
        if (car->lineSensor.check_if_initialized()) {
            car->lineSensor.reset();
        }

        car->imu.gyro.reset();
        car->motors.set_movement(Movement_creator::turn_degrees(car->imu.gyro.turn_angle.z + 360));

        while (car->motors.current_movement.action != Action::NO_ACTION) {
            car->update();
            car->lineSensor.calibrate_sensors();
        }
    }

    void reset() {
        nodes = 0;
    }

    void explore_maze() {
        reset();

        follow_segment();
        drive_to_intersection_center();


        add_node_and_edges();

        // while there still are unknown edges:
            // get the closest unknown edge and search it
            // add_edge_end
    }

    void drive_shortest_loop() {
        // find the shortest loop and drive it
    }

    void drive_loop_to_battery_charger() {
        // find loop including battery charger and drive it
    }

    void follow_segment() {
        // line follower
    }

    void drive_to_intersection_center() {
        // drive forward 49 mm
    }

    void add_node_and_edges() {
        add_node();

        car->motors.set_movement(Movement_creator::turn_degrees(360));
        while (car->motors.current_movement.action != Action::NO_ACTION) {
            if (car->lineSensor.line_under_middle) {
                add_unknown_edge(nodes - 1,car->motors.rotations_degrees);
            }
        }
    }

    void add_node() {
        Vector_2<float> coordinates = car->motors.distance_xy;
        uint8_t edges_index[4] = {NO_EDGE_INDEX, NO_EDGE_INDEX, NO_EDGE_INDEX, NO_EDGE_INDEX};
        float edge_rotations[4] = {0};

        path[nodes] = Node(coordinates, edges_index, edge_rotations);
    }

    void add_edge_end(uint8_t start_node, uint8_t end_node, uint32_t length, float start_rotation, float end_rotation) {
        // path[start_node].edges[edge_index] = Edge {length, start_node, end_node};
        // path[start_node].edge_rotations[edge_index] = fmod(start_rotation, 360.0);

        // path[end_node].edges[edge_index] =  Edge {length, end_node, start_node};
        // path[end_node].edge_rotations[edge_index] = fmod((end_rotation + 180.0), 360.0);
    }

    void add_unknown_edge(uint8_t current_node, float rotation) {
        Node node = path[current_node];
        uint8_t current_edge_index = node.edges_index[node.edge_amount];
        edges[current_edge_index] = unknown_edge;
        node.edge_rotations[node.edge_amount] = rotation;
    }
};

#endif // ZUMO_ROBOT_LINE_FOLLOWER_H