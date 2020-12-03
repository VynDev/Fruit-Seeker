#include <iostream>
#include "Project.h"
#include "Game.h"
#include "Map.h"
#include "Player.h"
#include "Fruit.h"
#include "EnvGine/Text.h"
#include "EnvGine/Surface.h"
#include "AI.h"

using namespace std::chrono;

int Game::nextId = 0;
std::vector<Position> Game::fruitPositionPossibilities;

Game::Game(Engine *engine) : Environment(engine) {
    id = nextId;
    ++nextId;

    map = CreateEntity<Map>();
    player = CreateEntity<Player>();
    fruit = CreateEntity<Fruit>();

    map->SetPosition(0, 0);
    map->SetWidthCaseCount(16);
    map->SetHeightCaseCount(16);

    mapSize = map->GetWidthCaseCount() * map->GetHeightCaseCount();

    if (fruitPositionPossibilities.size() == 0) {
        std::uniform_int_distribution<int> distributionWeight(0, map->GetWidthCaseCount() - 1);
        std::uniform_int_distribution<int> distributionHeight(0, map->GetHeightCaseCount() - 1);
        for (int i = 0; i < 1000; ++i) {
            Position pos;
            pos.x = distributionWeight(generator);
            pos.y = distributionHeight(generator);
            fruitPositionPossibilities.push_back(pos);
        }
    }

    /*fpsText = CreateEntity<Text>();
    fpsText->SetPosition(2, 2);
    fpsText->SetCharacterSize(12);
    fpsText->SetText("fps: -");*/

    scoreText = CreateEntity<Text>();
    //scoreText->SetPosition(2 + fpsText->GetWidth() * 2, 2);
    scoreText->SetPosition(2, 2);
    scoreText->SetCharacterSize(12);

    /*idText = CreateEntity<Text>();
    idText->SetPosition(2, fpsText->GetHeight() * 2);
    idText->SetCharacterSize(12);
    idText->SetText(std::string("id: ") + std::to_string(id));*/
    
    SetPosition(0, 0);
    OnSurfaceResized(512, 512);

    Setup();
}

void Game::Setup() {
    afkCycle = 0;
    player->SetCaseX(5);
    player->SetCaseY(5);
    player->SetScore(0);
    player->SetLength(0);

    fruit->SetCaseX(3);
    fruit->SetCaseY(3);

    player->SetX(player->GetCaseX() * map->GetCaseWidth());
    player->SetY(player->GetCaseY() * map->GetCaseHeight());
    fruit->SetX(fruit->GetCaseX() * map->GetCaseWidth());
    fruit->SetY(fruit->GetCaseY() * map->GetCaseHeight());

    scoreText->SetText("Score: 0");
}

void Game::Reset() {
    Setup();
}

void Game::Tick(float delta)  {
    deltaAccumulation += delta;

    if (deltaAccumulation < gameSpeed)
        return ;

    ++afkCycle;

    if (afkCycle > 100) {
        SetRunning(false);
    }

    /* Process player movements */
    if (ai)
        player->Move(ai->GetDirection(*this));
    else
        player->Move(player->GetMoveDirection());

    player->SetPosition(player->GetCaseX() * map->GetCaseWidth(), player->GetCaseY() * map->GetCaseHeight());

    /* Check game state */
    if (player->GetX() == fruit->GetX() && player->GetY() == fruit->GetY())
    {
        //player->AddLength();
        player->AddScore(1);
        scoreText->SetText(std::string("Score: ") + std::to_string(player->GetScore()));
        RespawnFruit();
        afkCycle = 0;
    }

    player->AddScore(0.00001);

    //fpsText->SetText(std::string("fps: ") + std::to_string(GetFpsCount()));

    if (!CheckSnake())
        SetRunning(false);

    deltaAccumulation = 0;
}

void Game::RespawnFruit() {
    bool caseIsNotEmpty;
    int tryCount = 0;
    int newfruitX = -1;
    int newfruitY;

    if (player->GetScore() < 1000) {
        bool ok = false;
        int k = 0;
        while (!ok && player->GetScore() + k < 1000) {
            ok = true;
            for (int i = 0; i < player->GetLength(); ++i) {
                const Position &previousPos = player->GetPreviousBodyPos(i + 1);
                if (previousPos.x == fruitPositionPossibilities[player->GetScore() + k].x && previousPos.y == fruitPositionPossibilities[player->GetScore() + k].y) {
                    ok = false;
                }
            }
            if (ok) {
                newfruitX = fruitPositionPossibilities[player->GetScore() + k].x;
                newfruitY = fruitPositionPossibilities[player->GetScore() + k].y;
            }
            ++k;
        }
    }

    if (newfruitX == -1) {
        Position pos = GenerateFruitPosition();
        newfruitX = pos.x;
        newfruitY = pos.y;
    }

    fruit->SetCaseX(newfruitX);
    fruit->SetCaseY(newfruitY);
    fruit->SetX(newfruitX * map->GetCaseWidth());
    fruit->SetY(newfruitY * map->GetCaseHeight());
    //std::cout << "Respawn fruit pos: " << newfruitX << ", " << newfruitY << std::endl;
}

Position Game::GenerateFruitPosition() {
    bool caseIsNotEmpty;
    int tryCount = 0;
    Position pos;

    do {
        caseIsNotEmpty = false;
        std::uniform_int_distribution<int> distributionWeight(0, map->GetWidthCaseCount() - 1);
        std::uniform_int_distribution<int> distributionHeight(0, map->GetHeightCaseCount() - 1);
        pos.x = distributionWeight(generator);
        pos.y = distributionHeight(generator);
        
        for (int i = 0; i < player->GetLength(); ++i) {
            const Position &previousPos = player->GetPreviousBodyPos(i + 1);
            if (previousPos.x == pos.x && previousPos.y == pos.y)
                caseIsNotEmpty = true;
        }
        ++tryCount;
        //std::cout << "Respawn fruit try: " << tryCount << std::endl;
    } while (caseIsNotEmpty && tryCount < 5);
    return pos;
}

bool Game::CheckSnake() {
    for (int i = 0; i < player->GetLength(); ++i) {
        const Position &previousPos = player->GetPreviousBodyPos(i + 1);
        if (previousPos.x == player->GetCaseX() && previousPos.y == player->GetCaseY())
            return false;
    }

    if (player->GetX() < 0 || player->GetX() >= map->GetWidth() || player->GetY() < 0 || player->GetY() >= map->GetHeight()) { // Check map borders
        //std::cout << "Snake out of the map" << std::endl;
        return false;
    }

    return true;
}

Player &Game::GetPlayer() {
    return *player;
}

void Game::OnSurfaceResized(int newWidth, int newHeight) {

    map->SetWidth(newWidth);
    map->SetHeight(newHeight);

    player->SetWidth(map->GetCaseWidth());
    player->SetHeight(map->GetCaseHeight());

    fruit->SetWidth(map->GetCaseWidth());
    fruit->SetHeight(map->GetCaseHeight());

    player->SetX(player->GetCaseX() * map->GetCaseWidth());
    player->SetY(player->GetCaseY() * map->GetCaseHeight());
    fruit->SetX(fruit->GetCaseX() * map->GetCaseWidth());
    fruit->SetY(fruit->GetCaseY() * map->GetCaseHeight());
}

void Game::SetAI(AI *ai) {
    this->ai = ai;
}