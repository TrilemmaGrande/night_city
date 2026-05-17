//
// Created by trilemma on 17.05.26.
//

#include "Building.h"

Building::Building(float x, float z, float _width, float _length) {
    height = GetRandomValue(1000, 3000) / 125;
    width = _width;
    length = _length;
    Color neon = {
        25,
        (unsigned char) GetRandomValue(100, 255),
        (unsigned char) GetRandomValue(150, 255),
        255
    };
    color = neon;
    position = {x, height / 2.0f, z};
}

void Building::draw() const {
    DrawCubeV(
        position,
        {
            width,
            height,
            length
        },
        BLACK
    );

    DrawCubeWiresV(
        position,
        {
            width + lineSeparation,
            height + lineSeparation,
            length + lineSeparation
        },
        color
    );
}
