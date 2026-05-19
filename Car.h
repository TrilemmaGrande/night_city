//
// Created by trilemma on 17.05.26.
//

#ifndef NIGHT_CITY_CAR_H
#define NIGHT_CITY_CAR_H
#include <raylib.h>

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
    CarType type{};
    float height = 0.1f;
    float width = 0.5f;
    float length = 0.12f;
    float speed = 0.001f;
    Vector3 position{};
    Direction direction{};
    Vector3 vDirection{};
    Vector3 front{};
    Vector3 rear{};
    Vector3 trailPos{};
    Color color{};
    Color frontColor{};
    Color rearColor{};

    void setupType();

    void setupSize();

public:
    Car(::Direction direction, float x, float flyHeight, float z);

    void move(const int &radius);

    void draw() const;
};


#endif //NIGHT_CITY_CAR_H
