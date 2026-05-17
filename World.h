//
// Created by trilemma on 17.05.26.
//

#ifndef NIGHT_CITY_WORLD_H
#define NIGHT_CITY_WORLD_H
#include <vector>

#include "Building.h"
#include "Car.h"
#include "Config.h"


class World {
public:
    static Camera3D initCamera();

    static void generateWorld(std::vector<Building> &buildings, std::vector<Car> &cars, const Config &cfg);
};


#endif //NIGHT_CITY_WORLD_H
