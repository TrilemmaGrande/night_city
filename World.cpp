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
                          const Config& cfg)
{
    float laneOffsetOuter = cfg.laneOffsetOuter;
    float laneOffsetInner = cfg.laneOffsetInner;
    int streetSpacing = cfg.streetSpacing;
    int spawnRadius = cfg.spawnRadius;

    for (int x = -spawnRadius; x < spawnRadius; x += 2) {
        for (int z = -spawnRadius; z < spawnRadius; z += 2) {

            bool skipBuilding = GetRandomValue(0, 5) == 0;
            bool skipVehicle = GetRandomValue(0, 1) == 0;

            if ((x % streetSpacing == 0) && (z % streetSpacing == 0)) {
                if (skipBuilding) continue;
                buildings.emplace_back(x, z, 2, 2);
                continue;
            }

            if (skipVehicle) continue;

            bool streetX = (x % streetSpacing != 0);
            bool streetZ = (z % streetSpacing != 0);
            float laneOffsetNorthSouth = 1.0f;
            float laneOffsetEastWest = -1.0f;
            float flyHeightLow = 1.2f;
            float flyHeightMid = 4.4f;
            float flyHeightHigh = 12.0f;


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
