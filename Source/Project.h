#pragma once

#define MIN_ENV_WIDTH 140
#define MAX_ENV 1000
#include <vector>

class Environment;
class Game;

struct ProjectEnvironments {
    Environment *menu = nullptr;
    Environment *game = nullptr;
    std::vector<Game*> ais;
};

ProjectEnvironments *Project();