#include "AI.h"
#include "LearnNN.h"
#include "Map.h"
#include "Fruit.h"
#include "Project.h"

using namespace LearNN;

int AI::nextId = 0;

AI::AI() : neuralNet(8), id(nextId++) {
	neuralNet.AddDenseLayer<Sigmoid>(8);
    neuralNet.AddDenseLayer<Sigmoid>(16);
	neuralNet.AddDenseLayer<Sigmoid>(4);
}

NeuralNetwork &AI::GetNeuralNetwork() {
    return neuralNet;
}

int GetIndexMaxValue(std::vector<double> v) {
    double maxValue = 0;
    int index;
    bool firstLoop = true;
    for (int i = 0; i < v.size(); ++i) {
        if (firstLoop || v[i] > maxValue) {
            firstLoop = false;
            maxValue = v[i];
            index = i;
        }
    }
    return index;
}

int AI::GetDirection(Game &game) {
    Input neuralNetInput = CalcNeuralNetInput_(game);
    Input v = neuralNet.CalculateOutput(neuralNetInput);

    if (id == Project()->ais[0]->GetAI()->GetId())
    {
        //std::cout << "A: " << neuralNetInput << std::endl;
    }

    int direction_ = GetIndexMaxValue(v) + 1;

    if (direction_ == DIRECTION_UP && lastDirection != DIRECTION_DOWN) {
        lastDirection = DIRECTION_UP;
    } else if (direction_ == DIRECTION_LEFT  && lastDirection != DIRECTION_RIGHT) {
        lastDirection = DIRECTION_LEFT;
    } else if (direction_ == DIRECTION_DOWN   && lastDirection != DIRECTION_UP) {
        lastDirection = DIRECTION_DOWN;
    } else if (direction_ == DIRECTION_RIGHT  && lastDirection != DIRECTION_LEFT) {
        lastDirection = DIRECTION_RIGHT;
    }

    return lastDirection;
}

/*  Input map:
*
*   0: UP distance to the wall
*   1: LEFT distance to the wall
*   2: DOWN distance to the wall
*   3: RIGHT distance to the wall
*
*   4: UP distance to the body
*   5: LEFT distance to the body
*   6: DOWN distance to the body
*   7: RIGHT distance to the body
*
*   8: UP distance to the fruit
*   9: LEFT distance to the fruit
*   10: DOWN distance to the fruit
*   11: RIGHT distance to the fruit
*
*/

Input AI::CalcNeuralNetInput_(Game &game) {
    Input NeuralNetInput;

    SetWallsDistance_(game, NeuralNetInput);
    //SetBodyDistance_(game, NeuralNetInput);
    SetFruitDistance_(game, NeuralNetInput);

    return NeuralNetInput;
}

void AI::SetWallsDistance_(Game &game, Input &NeuralNetInput) {
    const Player &player = game.GetPlayer();

    NeuralNetInput.push_back((double)player.GetCaseX() / (double)game.GetMap().GetWidthCaseCount());
    NeuralNetInput.push_back((double)player.GetCaseY() / (double)game.GetMap().GetHeightCaseCount());
    NeuralNetInput.push_back((double)(game.GetMap().GetWidthCaseCount() - player.GetCaseX()) / (double)game.GetMap().GetWidthCaseCount());
    NeuralNetInput.push_back((double)(game.GetMap().GetHeightCaseCount() - player.GetCaseY()) / (double)game.GetMap().GetHeightCaseCount());
}

void AI::SetBodyDistance_(Game &game, Input &NeuralNetInput) {
    const Player &player = game.GetPlayer();

    bool right = false;
    bool left = false;
    bool up = false;
    bool down = false;
    for (int i = 0; i < player.GetLength(); ++i) {
        if (!right && player.GetPreviousBodyPos(i).x > player.GetCaseX() && player.GetPreviousBodyPos(i).y == player.GetCaseY()) {
            NeuralNetInput.push_back(player.GetPreviousBodyPos(i).y - player.GetCaseY());
            right = true;
        }
    }
    if (!right)
        NeuralNetInput.push_back(2);

    for (int i = 0; i < player.GetLength(); ++i) {
        if (!left && player.GetPreviousBodyPos(i).x < player.GetCaseX() && player.GetPreviousBodyPos(i).y == player.GetCaseY()) {
            NeuralNetInput.push_back(player.GetCaseY() - player.GetPreviousBodyPos(i).y);
            left = true;
        }
    }
    if (!left)
        NeuralNetInput.push_back(2);

    for (int i = 0; i < player.GetLength(); ++i) {
        if (!up && player.GetPreviousBodyPos(i).y > player.GetCaseY() && player.GetPreviousBodyPos(i).x == player.GetCaseX()) {
            NeuralNetInput.push_back(player.GetPreviousBodyPos(i).x - player.GetCaseX());
            up = true;
        }
    }
    if (!up)
        NeuralNetInput.push_back(2);

    for (int i = 0; i < player.GetLength(); ++i) {

        if (!down && player.GetPreviousBodyPos(i).y < player.GetCaseY() && player.GetPreviousBodyPos(i).x == player.GetCaseX()) {
            NeuralNetInput.push_back(player.GetCaseX() - player.GetPreviousBodyPos(i).x);
            down = true;
        }
    }
    if (!down)
        NeuralNetInput.push_back(2);
}

void AI::SetFruitDistance_(Game &game, Input &NeuralNetInput) {
    const Player &player = game.GetPlayer();

    if (player.GetCaseY() == game.GetFruit().GetCaseY() && player.GetCaseX() > game.GetFruit().GetCaseX()) {
        NeuralNetInput.push_back(1);
    } else {
        NeuralNetInput.push_back(0);
    }

    if (player.GetCaseY() == game.GetFruit().GetCaseY() && player.GetCaseX() < game.GetFruit().GetCaseX()) {
        NeuralNetInput.push_back(1);
    } else {
        NeuralNetInput.push_back(0);
    }

    if (player.GetCaseX() == game.GetFruit().GetCaseX() && player.GetCaseY() > game.GetFruit().GetCaseY()) {
        NeuralNetInput.push_back(1);
    } else {
        NeuralNetInput.push_back(0);
    }

    if (player.GetCaseX() == game.GetFruit().GetCaseX() && player.GetCaseY() < game.GetFruit().GetCaseY()) {
        NeuralNetInput.push_back(1);
    } else {
        NeuralNetInput.push_back(0);
    }

    //NeuralNetInput.push_back(((double)player.GetCaseX() - game.GetFruit().GetCaseX()) / (double)game.GetMap().GetWidthCaseCount());
   // NeuralNetInput.push_back(((double)player.GetCaseY() - game.GetFruit().GetCaseY()) / (double)game.GetMap().GetHeightCaseCount());
}

void AI::Reset() {
    lastDirection = 0;
}
