//
// Created by trilemma on 17.05.26.
//

#include "Car.h"

#include <raylib.h>
#include <raymath.h>

Car::Car(const Direction direction, const float x, const float flyHeight, const float z) {
    this->direction = direction;
    this->type = CarType(GetRandomValue(0, 3));
    float jitter = ((float) GetRandomValue(-1000, 1000) / 3333.0f);
    this->position = {x + jitter, flyHeight + (jitter / 2), z + jitter};
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
        float s = width;
        width = length;
        length = s;
    }
}


void Car::move(int &radius) {
    position = Vector3Add(position, Vector3Scale(vDirection, speed));

    if (position.x < -radius) position.x = radius;
    if (position.x > radius) position.x = -radius;
    if (position.z < -radius) position.z = radius;
    if (position.z > radius) position.z = -radius;
}

void Car::draw() {
    DrawCube(position, width, height, length, color);
    // Front light
    Vector3 front = position;

    if (direction == NORTH) front.z -= length / 2;
    if (direction == SOUTH) front.z += length / 2;
    if (direction == EAST) front.x += width / 2;
    if (direction == WEST) front.x -= width / 2;
    Color c = {
        200,
        200,
        255,
        255
    };
    DrawCubeWiresV(front, {0.05f, 0.05f, 0.05f}, Fade(c, 0.9f));

    // Rear light
    Vector3 back = position;

    if (direction == NORTH) back.z += length / 2;
    if (direction == SOUTH) back.z -= length / 2;
    if (direction == EAST) back.x -= width / 2;
    if (direction == WEST) back.x += width / 2;

    c = {
        255,
        50,
        50,
        120
    };
    DrawCubeWiresV(back, {0.05f, 0.05f, 0.05f}, Fade(c, 0.9f));

    trail.push_back(back);

    if (trail.size() > 5)
        trail.erase(trail.begin());


    for (int i = 1; i < trail.size(); i++) {
        float t = (float) trail.size() / (float) i;
        float size = 0.1f - i / 100;
        DrawCubeWiresV(trail[i], {size, size, size}, Fade(c, t));
    }
}
