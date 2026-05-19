//
// Created by trilemma on 17.05.26.
//

#include "Car.h"
#include <raylib.h>
#include <raymath.h>

Car::Car(const Direction direction, const float x, const float flyHeight, const float z): direction(direction) {
    this->type = static_cast<CarType>(GetRandomValue(0, 3));
    const float jitter = static_cast<float>(GetRandomValue(-1000, 1000)) / 3333.0f;
    this->position = {x + jitter, flyHeight + (jitter / 2), z + jitter};
    this->frontColor = {
        200,
        200,
        255,
        255
    };
    this->rearColor = {
        255,
        50,
        50,
        120
    };

    switch (direction) {
        case NORTH: vDirection = {0, 0, -1};
            break;
        case SOUTH: vDirection = {0, 0, 1};
            break;
        case EAST: vDirection = {1, 0, 0};
            break;
        case WEST: vDirection = {-1, 0, 0};
            break;
    }
    setupType();
    setupSize();
}

void Car::setupType() {
    switch (type) {
        case NORMAL:
            speed = 0.01f;
            width = 0.12f;
            height = 0.1f;
            length = 0.5f;
            color = GRAY;
            break;

        case SPORT:
            speed = 0.03f;
            width = 0.10f;
            height = 0.06f;
            length = 0.5f;
            color = DARKBLUE;
            break;

        case TRUCK:
            speed = 0.005f;
            width = 0.15f;
            height = 0.4f;
            length = 1.2f;
            color = DARKBROWN;
            break;

        case TAXI:
            speed = 0.02f;
            width = 0.12f;
            height = 0.2f;
            length = 0.5f;
            color = ORANGE;
            break;
    }
}

void Car::setupSize() {
    if (direction == EAST || direction == WEST) {
        auto const tmp = width;
        width = height;
        height = tmp;
    }
}


void Car::move(const int &radius) {
    position = Vector3Add(position, Vector3Scale(vDirection, speed));

    if (position.x < -radius) position.x = radius;
    if (position.x > radius) position.x = -radius;
    if (position.z < -radius) position.z = radius;
    if (position.z > radius) position.z = -radius;

    front = position;
    if (direction == NORTH) front.z -= length / 2;
    if (direction == SOUTH) front.z += length / 2;
    if (direction == EAST) front.x += width / 2;
    if (direction == WEST) front.x -= width / 2;

    rear = position;
    if (direction == NORTH) { rear.z += length / 2; }
    if (direction == SOUTH) { rear.z -= length / 2; }
    if (direction == EAST) { rear.x -= width / 2; }
    if (direction == WEST) { rear.x += width / 2; }

    trailPos = {
        rear.x - vDirection.x * speed * 15,
        rear.y - vDirection.y * speed * 15,
        rear.z - vDirection.z * speed * 15
    };
}

void Car::draw() const {
    DrawCube(position, width, height, length, color);
    DrawCubeWiresV(front, {0.05f, 0.05f, 0.05f}, frontColor);
    DrawCubeWiresV(rear, {0.05f, 0.05f, 0.05f}, rearColor);
    DrawLine3D(position, trailPos, rearColor);
}
