#include <format>
#include <vector>

#include "raylib.h"
#include "raymath.h"

constexpr int SPAWNRADIUS = 22;

struct Building {
    Vector3 position{};
    Color color{};
    float height;
    float width;
    float length;
    float lineSeparation = 0.001f;

    Building(float x, float z, float _width, float _length) {
        height = GetRandomValue(1000, 3000) / 125;
        width = _width;
        length = _length;
        Color neon = {
            25,
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
    Vector3 vDirection{};
    std::vector<Vector3> trail{};
    float height = 0.1f;
    float width = 0.5f;
    float length = 0.12f;
    float speed = 0.001f;
    Color color{};

    void setupType() {
        switch (type) {
            case NORMAL:
                speed = 0.01f;
                width = 0.12f;
                height = 0.1f;
                length = 0.5f;
                color = GRAY;
                break;

            case SPORT:
                speed = 0.03f;
                width = 0.10f;
                height = 0.06f;
                length = 0.5f;
                color = DARKBLUE;
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
                color = ORANGE;
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
        switch (direction) {
            case NORTH: vDirection = {0,0,-1};
                break;
            case SOUTH: vDirection = {0,0,1};
                break;
            case EAST: vDirection = {1,0,0};
                break;
            case WEST: vDirection = {-1,0,0};
                break;
        }
        setupType();
        setupSize();
    }

    void move() {
        position = Vector3Add(position, Vector3Scale(vDirection, speed));

        if (position.x < -SPAWNRADIUS) position.x = SPAWNRADIUS;
        if (position.x > SPAWNRADIUS) position.x = -SPAWNRADIUS;
        if (position.z < -SPAWNRADIUS) position.z = SPAWNRADIUS;
        if (position.z > SPAWNRADIUS) position.z = -SPAWNRADIUS;
    }

    void draw() {
        DrawCube(position, width, height, length, color);
        // Front light
        Vector3 front = position;

        if (direction == NORTH) front.z -= length / 2;
        if (direction == SOUTH) front.z += length / 2;
        if (direction == EAST) front.x += width / 2;
        if (direction == WEST) front.x -= width / 2;
        Color c = {
            200,
            200,
            255,
            255
        };
        DrawCubeWiresV(front, {0.05f, 0.05f, 0.05f}, Fade(c, 0.9f));

        // Rear light
        Vector3 back = position;

        if (direction == NORTH) back.z += length / 2;
        if (direction == SOUTH) back.z -= length / 2;
        if (direction == EAST) back.x -= width / 2;
        if (direction == WEST) back.x += width / 2;

        c = {
            255,
            50,
            50,
            120
        };
        DrawCubeWiresV(back, {0.05f, 0.05f, 0.05f}, Fade(c, 0.9f));

        trail.push_back(back);

        if (trail.size() > 5)
            trail.erase(trail.begin());


        for (int i = 1; i < trail.size(); i++) {
            float t = (float) trail.size() / (float) i;
            float size = 0.1f - i / 100;
            DrawCubeWiresV(trail[i], {size, size, size}, Fade(c, t));
        }
    }
};

constexpr int STREET_SPACING = 4;

void generateWorld(std::vector<Building> &buildings,
                   std::vector<Car> &cars) {
    float laneOffsetOuter = 0.5f;
    float laneOffsetInner = 0.3f;

    for (int x = -SPAWNRADIUS; x < SPAWNRADIUS; x += 2) {
        for (int z = -SPAWNRADIUS; z < SPAWNRADIUS; z += 2) {
            bool skipBuilding = GetRandomValue(0, 5) == 0;
            bool skipVehicle = GetRandomValue(0, 1) == 0;
            if ((x % STREET_SPACING == 0) && (z % STREET_SPACING == 0)) {
                if (skipBuilding) continue;
                buildings.emplace_back(x, z, 2, 2);
                continue;
            }
            if (skipVehicle) continue;

            bool streetX = (x % STREET_SPACING != 0);
            bool streetZ = (z % STREET_SPACING != 0);
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

int main() {
    std::vector<Car> cars;
    std::vector<Building> buildings;
    generateWorld(buildings, cars);
    int screenWidth = 1200;
    int screenHeight = 1024;
    InitWindow(screenWidth, screenHeight, "Night City");

    Camera3D camera = {0};
    camera.position = {2.0f, 40.0f, 40.0f};
    camera.target = {0.0f, 10, 0.0f};
    camera.up = {0.0f, 1.0f, 0.0f};
    camera.fovy = 70.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    DisableCursor();
    SetTargetFPS(144);

    while (!WindowShouldClose()) {
        UpdateCamera(&camera, CAMERA_ORBITAL);
        BeginDrawing();
        ClearBackground(BLACK);
        DrawPlane({0, 0, 0}, {150, 150}, Color{0, 0, 10, 255});

        for (auto &car: cars) {
            car.move();
        }
        BeginMode3D(camera);
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
