#pragma once

#include <iostream>
#include <functional>

#include <SFML/Graphics.hpp>

#include "EnvGine/Object.h"
#include "EnvGine/Engine.h"

class Map : public Object {

    public:

    Map(Environment *environment);

    void SetWidthCaseCount(int value);
    void SetHeightCaseCount(int value);
    int GetWidthCaseCount() const;
    int GetHeightCaseCount() const;

    int GetCaseWidth() const;
    int GetCaseHeight() const;

    private:

    int widthCaseCount;
    int heightCaseCount;

};