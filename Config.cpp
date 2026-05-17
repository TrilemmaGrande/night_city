//
// Created by trilemma on 17.05.26.
//

#include "Config.h"

#include <fstream>
#include <iosfwd>
#include <string>


void Config::loadConfig() {
    std::ifstream f("config.txt");
    if (!f.is_open()) return;

    std::string key;

    while (f >> key) {
        if (key == "SPAWNRADIUS") f >> spawnRadius;
        else if (key == "SPEED") f >> carSpeed;
        else if (key == "STREETSPACING") f >> streetSpacing;
        else if (key == "LANEOFFSETOUTER") f >> laneOffsetOuter;
        else if (key == "LANEOFFSETINNER") f >> laneOffsetInner;
        else if (key == "SCREENWIDTH") f >> screenWidth;
        else if (key == "SCREENHEIGHT") f >> screenHeight;
    }
}