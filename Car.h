//
// Created by trilemma on 17.05.26.
//

#ifndef NIGHT_CITY_CAR_H
#define NIGHT_CITY_CAR_H
#include <raylib.h>
#include <vector>

enum Direction {
    NORTH, EAST, SOUTH, WEST
};

enum CarType {
    NORMAL,
    SPORT,
    TRUCK,
    TAXI
};

class Car {
    Vector3 position{};
    CarType type{};
    Direction direction{};
    Vector3 vDirection{};
    std::vector<Vector3> trail{};
    float height = 0.1f;
    float width = 0.5f;
    float length = 0.12f;
    float speed = 0.001f;
    Color color{};

    void setupType();

    void setupSize();

public:
    Car(::Direction direction, float x, float flyHeight, float z);

    void move(int &radius);

    void draw();
};


#endif //NIGHT_CITY_CAR_H
