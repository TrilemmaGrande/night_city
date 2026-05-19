//
// Created by trilemma on 17.05.26.
//

#include "World.h"
#include "Config.h"

Camera3D World::initCamera() {
    Camera3D camera = {0};
    camera.position = {2.0f, 40.0f, 40.0f};
    camera.target = {0.0f, 10, 0.0f};
    camera.up = {0.0f, 1.0f, 0.0f};
    camera.fovy = 70.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    return camera;
}

void World::generateWorld(std::vector<Building> &buildings,
                          std::vector<Car> &cars,
                          const Config &cfg) {
    const float laneOffsetOuter = cfg.laneOffsetOuter;
    const float laneOffsetInner = cfg.laneOffsetInner;
    const int streetSpacing = cfg.streetSpacing;
    const int spawnRadius = cfg.spawnRadius;

    for (int x = -spawnRadius; x < spawnRadius; x += 2) {
        for (int z = -spawnRadius; z < spawnRadius; z += 2) {
            const bool skipBuilding = GetRandomValue(0, 5) == 0;
            const bool skipVehicle = GetRandomValue(0, 1) == 0;

            if ((x % streetSpacing == 0) && (z % streetSpacing == 0)) {
                if (skipBuilding) continue;
                buildings.emplace_back(x, z, 2, 2);
                continue;
            }

            if (skipVehicle) continue;

            const bool streetX = (x % streetSpacing != 0);
            const bool streetZ = (z % streetSpacing != 0);
            constexpr float flyHeightHigh = 12.0f;
            constexpr float flyHeightMid = 4.4f;
            constexpr float flyHeightLow = 1.2f;
            constexpr float laneOffsetNorthSouth = 1.0f;
            constexpr float laneOffsetEastWest = -1.0f;

            // NORTH SOUTH
            if (streetX) {
                // LEFT OUTER AND INNER LANE
                cars.emplace_back(NORTH, x - laneOffsetInner, flyHeightLow + laneOffsetNorthSouth, z);
                cars.emplace_back(SOUTH, x - laneOffsetOuter, flyHeightLow + laneOffsetNorthSouth, z);
                // RIGHT OUTER AND INNER LANE
                cars.emplace_back(SOUTH, x + laneOffsetInner, flyHeightLow + laneOffsetNorthSouth, z);
                cars.emplace_back(NORTH, x + laneOffsetOuter, flyHeightLow + laneOffsetNorthSouth, z);

                // JUST INNER LANE FOR VERY HIGH HIGHWAY
                cars.emplace_back(SOUTH, x + laneOffsetInner, flyHeightHigh + laneOffsetNorthSouth, z);
                cars.emplace_back(NORTH, x - laneOffsetInner, flyHeightHigh + laneOffsetNorthSouth, z);
                continue;
            }

            // EAST WEST
            if (streetZ) {
                // LEFT OUTER AND INNER LANE
                cars.emplace_back(EAST, x, flyHeightMid + laneOffsetEastWest, z - laneOffsetInner);
                cars.emplace_back(WEST, x, flyHeightMid + laneOffsetEastWest, z - laneOffsetOuter);
                // RIGHT OUTER AND INNER LANE
                cars.emplace_back(WEST, x, flyHeightMid + laneOffsetEastWest, z + laneOffsetInner);
                cars.emplace_back(EAST, x, flyHeightMid + laneOffsetEastWest, z + laneOffsetOuter);

                // JUST INNER LANE FOR VERY HIGH HIGHWAY
                cars.emplace_back(EAST, x, flyHeightHigh + laneOffsetEastWest, z - laneOffsetInner);
                cars.emplace_back(WEST, x, flyHeightHigh + laneOffsetEastWest, z + laneOffsetInner);
            }
        }
    }
}
