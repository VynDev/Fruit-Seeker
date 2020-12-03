#include "Project.h"
#include "Menu.h"
#include "GeneticAlgorithmMode.h"
#include "EnvGine/Engine.h"
#include "EnvGine/Text.h"
#include "EnvGine/Surface.h"
#include "Game.h"

Menu::Menu(Engine *engine_) : Environment(engine_) {
    FullScreen();

    Menu &menu = *this;
    
    Text *startNormalGameButton = this->CreateEntity<Text>();
    startNormalGameButton->SetText("Normal game");
    startNormalGameButton->CenterHorizontaly();
    startNormalGameButton->CenterVerticaly();
    startNormalGameButton->SetOnClickFunction([&]() {
        std::cout << "Normal game button triggered" << std::endl;
        if (Project()->game)
            Project()->game->Remove();
        Project()->game = GetEngine().CreateEnvironment<Game>();
        menu.SetRunning(false);
        GetEngine().StartEnvironment(Project()->game);
    });

    Text *startGeneticAlgorithmButton = this->CreateEntity<Text>();
    startGeneticAlgorithmButton->SetText("Genetic algorithm");
    startGeneticAlgorithmButton->CenterHorizontaly();
    startGeneticAlgorithmButton->CenterVerticaly();
    startGeneticAlgorithmButton->SetY(startGeneticAlgorithmButton->GetY() + startNormalGameButton->GetHeight());
    startGeneticAlgorithmButton->SetOnClickFunction([&]() {
        std::cout << "Genetic algorithm button triggered" << std::endl;
        StartGeneticAlgorithmMode(GetEnginePointer());
    });
}

void Menu::Tick(float delta) {
    
}