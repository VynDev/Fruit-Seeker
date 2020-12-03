#include "GeneticAlgorithmMode.h"
#include "EnvGine/Engine.h"
#include "Project.h"
#include "Game.h"
#include "GAMonitor.h"
#include "AI.h"

void StartGeneticAlgorithmMode(Engine *engine) {

    engine->SetTickEnabled(false);
    for (int i = 0; i < MAX_ENV; ++i) {
        Game *gameEnv = engine->CreateEnvironment<Game>();
        //std::cout << "created env " << i << std::endl;
        gameEnv->GetPlayer().SetInputsEnabled(false);
        gameEnv->SetAI(new AI);
        Project()->ais.push_back(gameEnv);
    }

    //Monitor *monitorEnv = engine->CreateEnvironment<Monitor>();
    //monitorEnv->SetWidth(engine->GetWindowWidth());
    //monitorEnv->SetHeight(engine->GetWindowHeight() / 6);
    //monitorEnv->SetPosition(0, engine->GetWindowHeight() - monitorEnv->GetHeight());
    //engine->StartEnvironment(monitorEnv);
    engine->SelectMonitor<GAMonitor>();

    for (int i = 0; i < MAX_ENV; ++i) {
        //std::cout << "starting env " << i << std::endl;
        engine->StartEnvironment(Project()->ais[i]);
    }

    engine->SetTickEnabled(true);
}