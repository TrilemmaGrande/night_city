#include <format>
#include <vector>

#include "raylib.h"

constexpr int SPAWNRADIUS = 20;
constexpr float PLAYER_NORMAL = 1.0f;

struct Building {
    Vector3 position{};
    Color color{};
    float height;
    float width;
    float length;
    float lineSeparation = 0.001f;

    Building(float x, float z, float _width, float _length) {
        height = GetRandomValue(1000, 3000)/150;
        width = _width;
        length = _length;
        Color neon = {
            (unsigned char) GetRandomValue(0, 50),
            (unsigned char) GetRandomValue(100, 255),
            (unsigned char) GetRandomValue(150, 255),
            255
        };
        color = neon;

        position = {x, height / 2.0f, z};
    }

    void draw() const {
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
};

enum Direction {
    NORTH, EAST, SOUTH, WEST
};

enum CarType {
    NORMAL,
    SPORT,
    TRUCK,
    TAXI
};

struct Car {
    Vector3 position{};
    CarType type;
    Direction direction;
    float height = 0.1f;
    float width = 0.5f;
    float length = 0.12f;
    float speed = 0.001f;
    Color color;

    void setupType() {
        switch (type) {
            case NORMAL:
                speed = 0.01f;
                width = 0.12f;
                height = 0.1f;
                length = 0.5f;
                color = DARKGRAY;
                break;

            case SPORT:
                speed = 0.03f;
                width = 0.10f;
                height = 0.06f;
                length = 0.5f;
                color = ORANGE;
                break;

            case TRUCK:
                speed = 0.005f;
                width = 0.15f;
                height = 0.4f;
                length = 1.2f;
                color = DARKBROWN;
                break;

            case TAXI:
                speed = 0.02f;
                width = 0.12f;
                height = 0.2f;
                length = 0.5f;
                color = YELLOW;
                break;
        }
    }

    void setupSize() {
        if (direction == EAST || direction == WEST) {
            float s = width;
            width = length;
            length = s;
        }
    }

    Car(const Direction direction, const float x, const float flyHeight, const float z) {
        this->direction = direction;
        this->type = CarType(GetRandomValue(0, 3));
        float jitter = ((float) GetRandomValue(-1000, 1000) / 3333.0f);
        this->position = {x + jitter, flyHeight + (jitter / 2), z + jitter};
        setupType();
        setupSize();
    }

    void move() {
        switch (direction) {
            case NORTH: position.z -= speed;
                break;
            case SOUTH: position.z += speed;
                break;
            case EAST: position.x += speed;
                break;
            case WEST: position.x -= speed;
                break;
        }

        if (position.x < -SPAWNRADIUS) position.x = SPAWNRADIUS;
        if (position.x > SPAWNRADIUS) position.x = -SPAWNRADIUS;
        if (position.z < -SPAWNRADIUS) position.z = SPAWNRADIUS;
        if (position.z > SPAWNRADIUS) position.z = -SPAWNRADIUS;
    }

    void draw() const {
        DrawCube(position, width, height, length, color);

        // Front light
        Vector3 front = position;

        if (direction == NORTH) front.z -= length / 2;
        if (direction == SOUTH) front.z += length / 2;
        if (direction == EAST) front.x += width / 2;
        if (direction == WEST) front.x -= width / 2;
        Color c = {
            (unsigned char)GetRandomValue(0, 50),
            (unsigned char)GetRandomValue(150, 255),
            (unsigned char)GetRandomValue(200, 255),
            (unsigned char)(std::ctype_base::alpha * 120)
        };
        DrawCube(front, 0.05f, 0.05f, 0.05f, Fade(c, 0.9f));

        // Rear light
        Vector3 back = position;

        if (direction == NORTH) back.z += length / 2;
        if (direction == SOUTH) back.z -= length / 2;
        if (direction == EAST) back.x -= width / 2;
        if (direction == WEST) back.x += width / 2;

        c = {
            (unsigned char)GetRandomValue(200, 255),
            (unsigned char)GetRandomValue(0, 50),
            (unsigned char)GetRandomValue(150, 255),
            (unsigned char)(std::ctype_base::alpha * 120)
        };
        DrawCube(back, 0.05f, 0.05f, 0.05f, Fade(c, 0.9f));
    }
};


constexpr int STREET_SPACING = 4;

bool isBuilding(int x, int z) {
    return (x % STREET_SPACING == 0) && (z % STREET_SPACING == 0);
}

void generateWorld(std::vector<Building> &buildings,
                   std::vector<Car> &cars) {
    float laneOffsetOuter = 0.5f;
    float laneOffsetInner = 0.3f;

    for (int x = -SPAWNRADIUS; x < SPAWNRADIUS; x += 2) {
        for (int z = -SPAWNRADIUS; z < SPAWNRADIUS; z += 2) {
            bool _isBuilding = isBuilding(x, z);

            if (_isBuilding) {
                buildings.push_back(Building(x, z, 2, 2));
                continue;
            }

            bool streetX = (x % STREET_SPACING != 0);
            bool streetZ = (z % STREET_SPACING != 0);

            float flyHeightLow = 1.0f;
            float flyHeightMid = 2.2f;
            float flyHeightHigh = 12.0f;

            // NORTH SOUTH
            if (streetX) {
                // LEFT OUTER AND INNER LANE
                cars.push_back(Car(NORTH, x - laneOffsetInner, flyHeightLow, z));
                cars.push_back(Car(SOUTH, x - laneOffsetOuter, flyHeightLow, z));
                // RIGHT OUTER AND INNER LANE
                cars.push_back(Car(SOUTH, x + laneOffsetInner, flyHeightLow, z));
                cars.push_back(Car(NORTH, x + laneOffsetOuter, flyHeightLow, z));

                // JUST INNER LANE FOR VERY HIGH HIGHWAY
                cars.push_back(Car(SOUTH, x + laneOffsetInner, flyHeightHigh, z));
                cars.push_back(Car(NORTH, x - laneOffsetInner, flyHeightHigh, z));
                continue;
            }

            // EAST WEST
            if (streetZ) {
                // LEFT OUTER AND INNER LANE
                cars.push_back(Car(EAST, x, flyHeightMid, z - laneOffsetInner));
                cars.push_back(Car(WEST, x, flyHeightMid, z - laneOffsetOuter));
                // RIGHT OUTER AND INNER LANE
                cars.push_back(Car(WEST, x, flyHeightMid, z + laneOffsetInner));
                cars.push_back(Car(EAST, x, flyHeightMid, z + laneOffsetOuter));

                // JUST INNER LANE FOR VERY HIGH HIGHWAY
                cars.push_back(Car(EAST, x, flyHeightHigh, z - laneOffsetInner));
                cars.push_back(Car(WEST, x, flyHeightHigh, z + laneOffsetInner));
            }
        }
    }
}

int main() {
    std::vector<Car> cars;
    std::vector<Building> buildings;
    generateWorld(buildings, cars);
int screenWidth = 1200;
    int screenHeight = 1024;
    InitWindow(screenWidth, screenHeight, "Night City");

    Camera3D camera = {0};
    camera.position = {2.0f, 20, 20.0f};
    camera.target = {2.0f, PLAYER_NORMAL, 0.0f};
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
            if (camera.position.y <= PLAYER_NORMAL) {
                camera.position.y = PLAYER_NORMAL;
                camera.target.y = PLAYER_NORMAL;
            }
        }
        UpdateCamera(&camera, CAMERA_ORBITAL);
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode3D(camera);

        DrawPlane({0, 0, 0}, {200, 200}, Color{20, 20, 20, 255});

        for (auto &car: cars) {
            car.move();
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
