#pragma once
#include "LearnNN.h"
#include "Player.h"
#include "Game.h"

class AI {

    public:

    static int nextId;

    AI();

    LearNN::NeuralNetwork &GetNeuralNetwork();
    int GetDirection(Game &game);
    void Reset();
    int GetId() const {return id;}

    private:

    int id = 0;
    
    LearNN::InputVector CalcNeuralNetInput_(Game &game);
    void SetWallsDistance_(Game &game, LearNN::InputVector &NeuralNetInput);
    void SetBodyDistance_(Game &game, LearNN::InputVector &NeuralNetInput);
    void SetFruitDistance_(Game &game, LearNN::InputVector &NeuralNetInput);

    LearNN::NeuralNetwork neuralNet;
    int lastDirection = 0;
};