//
// Created by trilemma on 17.05.26.
//

#include "Building.h"

Building::Building(float x, float z, float width, float length, Model model) : width(width), length(length), model(model) {
    height = GetRandomValue(1000, 3000) / 125;
    const Color neon = {
        25,
        static_cast<unsigned char>(GetRandomValue(100, 255)),
        static_cast<unsigned char>(GetRandomValue(150, 255)),
        255
    };
    color = neon;
    position = {x, height / 2.0f, z};
}

void Building::draw() const {
    DrawModelEx(model,
                position,
                {0, 1, 0}, 0,
                {
                    width,
                    height,
                    length
                },
                WHITE
    );
}
