//
// Created by trilemma on 17.05.26.
//

#include "World.h"
#include "Config.h"
#include "cmake-build-debug/_deps/raylib-src/src/raymath.h"

Camera3D World::initCamera() {
    Camera3D camera = {0};
    camera.position = {2.0f, 40.0f, 40.0f};
    camera.target = {0.0f, 10, 0.0f};
    camera.up = {0.0f, 1.0f, 0.0f};
    camera.fovy = 70.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    return camera;
}

Texture2D World::LoadTexture() {
    Image img = GenImageColor(128, 128, BLACK);

    const int windowW = 4;
    const int windowH = 2;

    const int gridX = 10;
    const int gridY = 16;

    for (int y = 0; y < gridY; y++) {
        for (int x = 0; x < gridX; x++) {

            bool on = GetRandomValue(0, 100) > 75;

            if (on) {
                int px = x * (128 / gridX);
                int py = y * (128 / gridY);

                ImageDrawRectangle(
                    &img,
                    px,
                    py,
                    windowW,
                    windowH/6,
                    Color{
                        220,
                        (unsigned char)GetRandomValue(200, 220),
                        (unsigned char)GetRandomValue(160, 220),
                        220
                    }
                );
            }
        }
    }

    return LoadTextureFromImage(img);
}

void World::GenerateWorld(std::vector<Building> &buildings,
                          std::vector<Car> &cars,
                          const Config &cfg) {
    const float laneOffsetOuter = cfg.laneOffsetOuter;
    const float laneOffsetInner = cfg.laneOffsetInner;
    const int streetSpacing = cfg.streetSpacing;
    const int gridRange = cfg.gridRange;
    for (int x = -gridRange; x <= gridRange; x += 2) {
        for (int z = -gridRange; z <= gridRange; z += 2) {
            const bool skipBuilding = GetRandomValue(0, 10) == 0;
            const bool skipVehicle = GetRandomValue(0, 1) == 0;

            if ((x % streetSpacing == 0) && (z % streetSpacing == 0)) {
                if (skipBuilding) continue;
                Texture2D texture = LoadTexture();
                SetTextureFilter(texture, TEXTURE_FILTER_POINT);
                Mesh cubeMesh = GenMeshCube(1.0f, 1.0f, 1.0f);
                Model model = LoadModelFromMesh(cubeMesh);
                model.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = texture;
                buildings.emplace_back(x, z, 2, 2, model);
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
