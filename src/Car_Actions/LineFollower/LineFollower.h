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
        nodes_amount = 0;
    }

    void explore_maze() {
        reset();

        drive_to_next_node();

        add_node();

        current_node_index = 0;
        current_node_distance = car->motors.distance;

        uint8_t next_unknown_edge_index = get_last_unknown_edge_index();
        while (next_unknown_edge_index != (uint8_t)-1) {
            drive_to_node(edges[next_unknown_edge_index].start_index);

            uint8_t current_node = edges[next_unknown_edge_index].start_index;

            for (int i = 0; i < 4; i++) {
                if (nodes[current_node].edges_index[i] == next_unknown_edge_index) {
                    car->motors.set_movement(Movement_creator::turn_degrees(car->motors.rotations_degrees + nodes[current_node].edge_rotations[i]));

                    while (car->motors.current_movement.action != Action::NO_ACTION) {
                        car->update();
                    }

                    drive_to_next_node();

                    add_node();

                    current_node_index = edges[next_unknown_edge_index].start_index;
                    current_node_distance = car->motors.distance;

                    break;
                }
            }

            next_unknown_edge_index = get_last_unknown_edge_index();
        }
    }

    uint8_t get_last_unknown_edge_index() {
        for (int i = edges_amount - 1; i > 0; i--) {
            if (edges[i].end_index == UNKNOWN_EDGE_INDEX) {
                return i;
            }
        }

        return -1;
    }

    uint8_t get_next_to_last_node(uint8_t start_node_index, uint8_t end_node_index, uint8_t ignore_edge_index = NO_EDGE_INDEX, bool ignore_battery_station = false) {
        uint8_t nodes_list_amount = 0;
        uint8_t nodes_list[25];
        uint8_t temp_node_list_amount = 0;
        uint8_t temp_node_list[25];

        bool visited_nodes[50] = {};
        visited_nodes[start_node_index] = true;

        for (uint8_t current_edge_index : nodes[start_node_index].edges_index) {
            uint8_t new_node_index = edges[current_edge_index].end_index;

            if (new_node_index == end_node_index) {
                return start_node_index;
            }


            if (new_node_index != NO_EDGE_INDEX) {
                nodes_list[nodes_list_amount] = edges[current_edge_index].end_index;
                nodes_list_amount++;
            }
        }

        while (true) {
            for (int i = 0; i < nodes_list_amount; i++) {
                visited_nodes[nodes_list[i]] = true;

                for (int j = 0; j < 4; j++) {
                    uint8_t new_node_index = edges[nodes[start_node_index].edges_index[i]].end_index;

                    if (new_node_index == ignore_edge_index or (ignore_battery_station and new_node_index == charging_station.end_node_edge_index)) {
                        continue;
                    }

                    if (new_node_index != NO_EDGE_INDEX and !visited_nodes[new_node_index]) {
                        if (new_node_index == end_node_index) {
                            return nodes_list[i];
                        }

                        temp_node_list[temp_node_list_amount] = edges[nodes[nodes_list[i]].edges_index[i]].end_index;
                        temp_node_list_amount++;
                    }
                }
            }

            memcpy(nodes_list, temp_node_list, sizeof(temp_node_list[0]) * 25);
        }
    }

    void drive_to_node(uint8_t node_index) {
        uint8_t path_list_amount = 0;
        uint8_t path_list[25];

        uint8_t start_node_index = current_node_index;
        uint8_t end_node_index = node_index;

        uint8_t result = get_next_to_last_node(start_node_index, end_node_index);

        while (result != start_node_index) {
            path_list[path_list_amount] = result;
            path_list_amount++;
            end_node_index = result;

            result = get_next_to_last_node(start_node_index, end_node_index);
        }

        follow_path(path_list, path_list_amount);
    }

    void drive_loop() {
        drive_to_node(charging_station.end_node_index);

        uint8_t path_list_amount = 0;
        uint8_t path_list[25];

        uint8_t start_node_index = current_node_index;
        uint8_t end_node_index = charging_station.start_node_index;

        uint8_t edge_start_end;

        for (unsigned char i : nodes[start_node_index].edges_index) {
            if (edges[i].end_index == charging_station.start_node_index or edges[i].start_index == charging_station.start_node_index) {
                edge_start_end = i;
            }
        }

        uint8_t result = get_next_to_last_node(start_node_index, end_node_index, edge_start_end,true);

        while (result != start_node_index) {
            path_list[path_list_amount] = result;
            path_list_amount++;
            end_node_index = result;

            result = get_next_to_last_node(start_node_index, end_node_index, charging_station.start_node_edge_index,true);
        }

        follow_path(path_list, path_list_amount);
    }

    void follow_path(const uint8_t *path, uint8_t path_length) {
        for (int i = path_length - 1; i >= 0; i++) {
            uint8_t new_path_index;

            for (int j = 0; j < 4; j++) {
                if (edges[nodes[current_node_index].edges_index[j]].end_index == path[i]) {
                    new_path_index = j;
                }
            }

            float path_rotation = nodes[current_node_index].edge_rotations[new_path_index];

            car->motors.set_movement(Movement_creator::turn_degrees(path_rotation - fmod(car->motors.rotations_degrees, 360.0)));

            while (car->motors.current_movement.action != Action::NO_ACTION) {
                car->update();
            }

            drive_to_next_node();
        }
    }

    void drive_loop_with_battery_charger() {
        drive_to_node(charging_station.start_node_index);

        float path_rotation = nodes[charging_station.start_node_index].edge_rotations[charging_station.start_node_edge_index];

        car->motors.set_movement(Movement_creator::turn_degrees(path_rotation - fmod(car->motors.rotations_degrees, 360.0)));

        while (car->motors.current_movement.action != Action::NO_ACTION) {
            car->update();
        }

        follow_segment(car->motors.distance + charging_station.distance_from_start_node);

        // battery_charger

        drive_to_next_node();

        drive_loop();
    }

    void drive_to_next_node() {
        follow_segment();
        drive_to_intersection_center();
    }

    void follow_segment(float distance = 0) {
        car->update();

        if (distance != 0) {
            if (car->motors.distance >= distance) {
                return;
            }
        }

        if (car->lineSensor.line_missing) {
            car->motors.set_movement(Movement_creator::move_distance(car->motors.distance + Car::CAR_LENGTH));


            while (car->motors.current_movement.action != Action::NO_ACTION) {
                car->update();
            }

            if (car->lineSensor.line_missing) {

                car->imu.gyro.reset();
                car->motors.set_movement(Movement_creator::turn_degrees(180));

                while (car->motors.current_movement.action != Action::NO_ACTION) {
                    car->update();
                }

                car->motors.set_movement(Movement_creator::move_distance(Car::CAR_LENGTH * 10));

                while (car->motors.current_movement.action != Action::NO_ACTION) {
                    car->update();
                }

                follow_segment();

                return;
            }
        }

        if (car->lineSensor.is_at_intersection) {
            return;
        }

        int16_t speed_difference = pid_controller_line_follower.get_pid_value(car->lineSensor.distance_from_center);

        int16_t left_speed = (int16_t)car->motors.max_speed + speed_difference;
        int16_t right_speed = (int16_t)car->motors.max_speed - speed_difference;

        left_speed = constrain(left_speed, 0, (int16_t)car->motors.max_speed);
        right_speed = constrain(right_speed, 0, (int16_t)car->motors.max_speed);

        car->motors.set_movement(Movement_creator::move(left_speed, right_speed));
    }

    void drive_to_intersection_center() {
        car->motors.set_movement(Movement_creator::move_distance(Car::CAR_LENGTH / 2));

        while (car->motors.current_movement.action != Action::NO_ACTION) {
            car->update();
        }
    }

    void add_node() {
        for (int i = 0; i < nodes_amount; i++) {
            int16_t error_margin = 100;
            if (car->motors.distance_xy.x - error_margin <= nodes[i].coordinates.x and nodes[i].coordinates.x <= car->motors.distance_xy.x + error_margin and
                car->motors.distance_xy.y - error_margin <= nodes[i].coordinates.y and nodes[i].coordinates.y <= car->motors.distance_xy.y + error_margin) {

                for (int j = 0; j < 4; j++) {
                    int16_t error_rotation = 10;
                    if (nodes[i].edge_rotations[j] - error_rotation <= car->motors.rotations_degrees and nodes[i].edge_rotations[j] + error_rotation <= car->motors.rotations_degrees) {
                        add_edge_end(current_node_index, i, car->motors.distance - current_node_distance, current_path_index, j, car->motors.rotations_degrees);
                    }
                }

                return;
            }
        }

        add_node_and_edges();
    }

    void add_node_and_edges() {
        add_new_node();

        car->motors.set_movement(Movement_creator::turn_degrees(360));
        while (car->motors.current_movement.action != Action::NO_ACTION) {
            car->update();
            if (car->lineSensor.line_under_middle) {
                add_unknown_edge(nodes_amount - 1, car->motors.rotations_degrees);
            }
        }
    }

    void add_new_node() {
        Vector_2<float> coordinates = car->motors.distance_xy;
        uint8_t edges_index[4] = {NO_EDGE_INDEX, NO_EDGE_INDEX, NO_EDGE_INDEX, NO_EDGE_INDEX};
        float edge_rotations[4] = {0};

        nodes[nodes_amount] = Node(coordinates, edges_index, edge_rotations);
    }

    void add_edge_end(uint8_t start_node, uint8_t end_node, float length, uint8_t start_node_edge_index, uint8_t end_node_edge_index, float end_rotation) {
        Edge new_edge {length, start_node, end_node};

        uint8_t current_edge_index = nodes[start_node].edges_index[start_node_edge_index];

        edges[current_edge_index] = new_edge;

        nodes[end_node].edges_index[end_node_edge_index] = edges_amount;
        nodes[end_node].edge_rotations[end_node_edge_index] = fmod(end_rotation + 180, 360.0);
    }

    void add_unknown_edge(uint8_t current_node, float rotation) {
        Edge new_edge {0, current_node, 100};
        edges[edges_amount] = new_edge;

        Node node = nodes[current_node];
        node.edges_index[node.edge_amount] = edges_amount;
        node.edge_rotations[node.edge_amount] = rotation;

        node.edge_amount++;
        edges_amount++;
    }
};

#endif // ZUMO_ROBOT_LINE_FOLLOWER_H