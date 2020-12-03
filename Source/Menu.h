#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "EnvGine/Environment.h"

class Menu : public Environment {

    public:

    Menu(Engine *engine);

    virtual void Tick(float delta);
    
    private:
};