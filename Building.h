//
// Created by trilemma on 17.05.26.
//

#ifndef NIGHT_CITY_BUILDING_H
#define NIGHT_CITY_BUILDING_H
#include <raylib.h>


class Building {
    Vector3 position{};
    Color color{};
    Model model{};
    float height;
    float width;
    float length;
    float lineSeparation = 0.001f;

public:
    Building(float x, float z, float width, float length,Model model);

    void draw() const;
};


#endif //NIGHT_CITY_BUILDING_H
