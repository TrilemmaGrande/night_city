#include <cmath>
#include "raylib.h"

constexpr float SPAWNRADIUS = 10.0f;
constexpr float MAPRADIUS = 100.0f;

struct Point {
    float x, y, z;
};

constexpr Point CENTER = Point{0.0f, 0.0f, 0.0f};
constexpr Vector3 V_CENTER = Vector3{CENTER.x, CENTER.y, CENTER.z};

void spawnInSpawnRadius(Camera &camera);

int main() {
    InitWindow(1280, 720, "Night City");

    Camera3D camera = {0};
    camera.position = {0.0f, 1.0f, 10.0f};
    camera.target = V_CENTER;
    camera.up = {0.0f, 1.0f, 0.0f};
    camera.fovy = 70.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    DisableCursor();

    SetTargetFPS(144);

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            camera.position.y++;
            camera.target.y++;
        }
        if (IsKeyDown(KEY_LEFT_CONTROL)) {
            camera.position.y--;
            camera.target.y--;
        }
        UpdateCamera(&camera, CAMERA_FIRST_PERSON);
        BeginDrawing();
        ClearBackground(SKYBLUE);

        BeginMode3D(camera);
        DrawGrid(100, 1.0f);
        spawnInSpawnRadius(camera);

        EndMode3D();

        DrawFPS(10, 10);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

void spawnInSpawnRadius(Camera &camera) {
    for (float x = -MAPRADIUS; x < MAPRADIUS; x += 4.0f) {
        for (float z = -MAPRADIUS; z < MAPRADIUS; z += 4.0f) {
            float dx = x - camera.position.x;
            float dz = z - camera.position.z;

            float distanceSquared = dx * dx + dz * dz;

            if (distanceSquared < SPAWNRADIUS * SPAWNRADIUS) {
                float radius = std::fmod(x * x + z * z, 14.0f) + 2.5f;
                float height = std::fmod(x * x + z * z, 12.5f) + 50.0f;

                DrawSphere(
                    {x, height, z},
                    radius,
                    WHITE
                );

                DrawCubeWires(
                    {x, 0 + ((height-50)/2 ), z},
                    2,
                    height - 50,
                    2,
                    MAROON
                );
            }
        }
    }
}
