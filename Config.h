//
// Created by trilemma on 17.05.26.
//

#ifndef NIGHT_CITY_CONFIG_H
#define NIGHT_CITY_CONFIG_H


class Config {
public:
    int spawnRadius = 20;
    float carSpeed = 0.001;
    int streetSpacing = 4;
    float laneOffsetOuter = 0.5f;
    float laneOffsetInner = 0.3f;
    int screenWidth = 1200;
    int screenHeight = 1024;

    void loadConfig();
};

#endif //NIGHT_CITY_CONFIG_H
