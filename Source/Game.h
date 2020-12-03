#pragma once
#include <random>
#include <chrono> 
#include <iostream>

#include <SFML/Graphics.hpp>

#include "Player.h"
#include "EnvGine/Environment.h"


#define STEP_INTERVAL 100 // ms
#define MAP_SIZE 16

class Map;
class Player;
class Fruit;
class AI;
class Text;

class Game : public Environment {
    public:

    /* Engine override functions */

    static int nextId;

    Game(Engine *engine);

    virtual void Tick(float delta) override;
    virtual void OnSurfaceResized(int newWidth, int newHeight) override;

    /* Game specific functions */

    void Setup();
    void Reset();
    Player &GetPlayer();
    Fruit &GetFruit() {return *fruit;}
    void SetAI(AI *ai);
    AI *GetAI() {return ai;}
    Map &GetMap() {return *map;}

    float GetGameSpeed() const {return gameSpeed;}
    void SetGameSpeed(float value) {gameSpeed = value;}

    /* Check */

    bool CheckSnake();

    /* Fruit */

    void RespawnFruit();
    Position GenerateFruitPosition();

    private:

    int id;

    Map *map = nullptr;
    Player *player = nullptr;
    Fruit *fruit = nullptr;
    AI *ai = nullptr;

    int mapSize;

    float deltaAccumulation = 0;

    Text *fpsText;
    int fpsCount = 0;

    Text *scoreText;
    Text *idText;

    int afkCycle = 0;
    float gameSpeed = 0.1;

    static std::vector<Position> fruitPositionPossibilities;

    std::default_random_engine generator;
};