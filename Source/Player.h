#pragma once

#include "EnvGine/Object.h"

#define DIRECTION_NONE 0
#define DIRECTION_UP 1
#define DIRECTION_LEFT 2
#define DIRECTION_DOWN 3
#define DIRECTION_RIGHT 4
#define MAX_POS_HISTORY_BUFFER_SIZE 128

struct Inputs {
    bool quit = false;
    int direction = DIRECTION_NONE;
};

class Player : public Object {

    public:

    /* Engine override functions */

    Player(Environment *environment);

    virtual void Tick(float delta) override;
    virtual void Render(Surface &surface) override;

    /* Snake specific functions */

    void Move(int direction);
    int GetMoveDirection() const;
    void SetMoveDirection(int direction);

    void SetScore(float score);
    void AddScore(float amount = 1);
    float GetScore() const;

    void AddLength(int amount = 1);
    int GetLength() const;
    void SetLength(int value);
    const Position &GetPreviousBodyPos(int before) const;

    void SetCaseX(int value);
    void SetCaseY(int value);
    int GetCaseX() const;
    int GetCaseY() const;

    void SetInputsEnabled(bool value) {inputsDisabled = !value;}

    protected:

    int caseX;
    int caseY;
    
    int lastDirection = DIRECTION_RIGHT;
    float score = 0;

    int posHistoryCursor = 0;
    Position posHistory[MAX_POS_HISTORY_BUFFER_SIZE];

    int length = 0;

    bool inputsDisabled = false;
};