#pragma once

#include "EnvGine/Monitor.h"
#include "AI.h"

class Text;

class GAMonitor : public Monitor {

    public:

    GAMonitor(Engine *engine);

    virtual void Tick(float delta) override;

    bool IsEnd() const;
    void ReproducePopulation(float elitePrct, float randomPrct);
    void RestartGames();
    void SortGames();
    void ReCalcEnvironmentsPosition();

    private:

    //Text *text = nullptr;
    //Text *gameSpeedText = nullptr;
    //Text *bestScoreText = nullptr;

    int currentGeneration = 1;
};