#include <iostream>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "EnvGine/Engine.h"
#include "EnvGine/Surface.h"
#include "EnvGine/Texture.h"

Player::Player(Environment *environment) : Object(environment) {
    Texture *texture = new Texture(128, 128);
    texture->GetSFMLRenderTexture().clear(sf::Color(200, 200, 200));
    SetTexture(texture);
}

void Player::Render(Surface &surface) {
    Object::Render(surface);
    for (int i = 0; i < GetLength(); ++i) {
        const Position &bodypos = GetPreviousBodyPos(i + 1);
        surface.Draw(&GetTexture(), bodypos.x * GetWidth(), bodypos.y * GetHeight(), GetWidth(), GetHeight());
    }
}

void Player::Tick(float delta) {
    if (inputsDisabled)
        return ;
    if (GetEngine().IsKeyPressed(sf::Keyboard::Key::Z) && lastDirection != DIRECTION_DOWN) {
        lastDirection = DIRECTION_UP;
    } else if (GetEngine().IsKeyPressed(sf::Keyboard::Key::Q) && lastDirection != DIRECTION_RIGHT) {
        lastDirection = DIRECTION_LEFT;
    } else if (GetEngine().IsKeyPressed(sf::Keyboard::Key::S)  && lastDirection != DIRECTION_UP) {
        lastDirection = DIRECTION_DOWN;
    } else if (GetEngine().IsKeyPressed(sf::Keyboard::Key::D) && lastDirection != DIRECTION_LEFT) {
        lastDirection = DIRECTION_RIGHT;
    }
}

void Player::Move(int direction) {
    posHistory[posHistoryCursor] = {.x = GetCaseX(), .y = GetCaseY()};
    ++posHistoryCursor;
    if (posHistoryCursor == MAX_POS_HISTORY_BUFFER_SIZE)
        posHistoryCursor = 0;
    
    switch (direction)
    {
        case DIRECTION_UP:
            SetCaseY(GetCaseY() - 1);
            break;
        case DIRECTION_LEFT:
            SetCaseX(GetCaseX() - 1);
            break;
        case DIRECTION_DOWN:
            SetCaseY(GetCaseY() + 1);
            break;
        case DIRECTION_RIGHT:
            SetCaseX(GetCaseX() + 1);
            break;
    }
    SetMoveDirection(direction);
}

const Position &Player::GetPreviousBodyPos(int before) const {
    int offset = posHistoryCursor - before;
    if (posHistory + offset < posHistory)
        offset += MAX_POS_HISTORY_BUFFER_SIZE;
    return *(posHistory + offset);
}

void Player::AddLength(int amount) {
    ++length;
}

int Player::GetLength() const {
    return length;
}


int Player::GetMoveDirection() const {
    return lastDirection;
}

void Player::SetMoveDirection(int direction)  {
    lastDirection = direction;
}

void Player::SetScore(float score) {
    this->score = score;
}

void Player::AddScore(float amount) {
    score += amount;
}

float Player::GetScore() const {
    return score;
}

void Player::SetLength(int value) {
    length = value;
}

void Player::SetCaseX(int value) {
    caseX = value;
}
void Player::SetCaseY(int value) {
    caseY = value;
}
int Player::GetCaseX() const {
    return caseX;
}

int Player::GetCaseY() const {
    return caseY;
}