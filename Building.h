//
// Created by trilemma on 17.05.26.
//

#ifndef NIGHT_CITY_BUILDING_H
#define NIGHT_CITY_BUILDING_H
#include <raylib.h>


class Building {
    Vector3 position{};
    Color color{};
    float height;
    float width;
    float length;
    float lineSeparation = 0.001f;

public:
    Building(float x, float z, float _width, float _length);

    void draw() const;
};


#endif //NIGHT_CITY_BUILDING_H
