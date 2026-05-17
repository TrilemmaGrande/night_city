#include <format>
#include <vector>

#include "Building.h"
#include "Car.h"
#include "raylib.h"
#include "World.h"

int main() {
    Config cfg;
    cfg.loadConfig();
    std::vector<Car> cars;
    std::vector<Building> buildings;
    World::generateWorld(buildings, cars, cfg);
    Camera3D camera = World::initCamera();
    int screenWidth = 1200;
    int screenHeight = 1024;
    InitWindow(screenWidth, screenHeight, "Night City");


    DisableCursor();
    SetTargetFPS(144);

    while (!WindowShouldClose()) {
        UpdateCamera(&camera, CAMERA_ORBITAL);
        BeginDrawing();
        ClearBackground(BLACK);
        for (auto &car: cars) {
            car.move(cfg.spawnRadius);
        }
        BeginMode3D(camera);
        DrawPlane({0, 0, 0}, {50, 50}, Color{0, 10, 10, 255});
        for (auto &car: cars) {
            car.draw();
        }
        for (auto &building: buildings) {
            building.draw();
        }
        EndMode3D();

        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
