#pragma once

#include "EnvGine/Object.h"

class Fruit : public Object {

    public:

    /* Engine override functions */

    Fruit(Environment *environment);

    /* Snake specific functions */

    void SetCaseX(int value) {caseX = value;}
    void SetCaseY(int value) {caseY = value;}
    int GetCaseX() const {return caseX;}
    int GetCaseY() const {return caseY;}

    private:

    int caseX;
    int caseY;
};