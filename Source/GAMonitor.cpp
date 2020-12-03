#include <iostream>
#include <cmath>
#include "GAMonitor.h"
#include "Project.h"
#include "EnvGine/Text.h"
#include "Game.h"

GAMonitor::GAMonitor(Engine *engine) : Monitor(engine) {
    Project()->menu->SetRunning(false);

    /*text = CreateEntity<Text>();
    text->SetPosition(0, 0);
    text->SetCharacterSize(16);

    bestScoreText = CreateEntity<Text>();
    bestScoreText->SetPosition(0, 32);
    bestScoreText->SetCharacterSize(16);

    gameSpeedText = CreateEntity<Text>();
    gameSpeedText->SetPosition(0, 64);
    gameSpeedText->SetCharacterSize(16);*/

    ReCalcEnvironmentsPosition();
}

void GAMonitor::Tick(float delta) {
    if (IsEnd()) {
        SortGames();
        ReCalcEnvironmentsPosition();
        ReproducePopulation(0.40, 0.35);
        RestartGames();
        ++currentGeneration;
        //text->SetText(std::string("Generation: ") + std::to_string(currentGeneration));
    }
    if (GetEngine()->IsKeyPressed(sf::Keyboard::Key::Z)) {
        for (int i = 0; i < Project()->ais.size(); ++i) {
            Project()->ais[i]->SetGameSpeed(Project()->ais[i]->GetGameSpeed() - 0.00005);
        }
        //gameSpeedText->SetText(std::string("Game speed interval: ") + std::to_string(Project()->ais[0]->GetGameSpeed()));
    }
    if (GetEngine()->IsKeyPressed(sf::Keyboard::Key::S)) {
        for (int i = 0; i < Project()->ais.size(); ++i) {
            Project()->ais[i]->SetGameSpeed(Project()->ais[i]->GetGameSpeed() + 0.00005);
        }
        //gameSpeedText->SetText(std::string("Game speed interval: ") + std::to_string(Project()->ais[0]->GetGameSpeed()));
    }
}

bool GAMonitor::IsEnd() const {
    for (int i = 0; i < MAX_ENV; ++i) {
        if (Project()->ais[i]->IsRunning())
            return false;
    }
    return true;
}

void GAMonitor::RestartGames() {
    for (int i = 0; i < MAX_ENV; ++i) {
        Project()->ais[i]->Reset();
        Project()->ais[i]->GetAI()->Reset();
        Project()->ais[i]->SetRunning(true);
        Project()->ais[i]->SetRenderingEnabled(i < 25);
    }
}

void GAMonitor::SortGames() {
    float maxScore = 0;
    std::vector<Game*> &population = Project()->ais;
    bool ok = false;
    while (!ok) {
        ok = true;
        for (int i = 0; i < population.size() - 1; ++i) {
            if (population[i]->GetPlayer().GetScore() > maxScore)
                maxScore = population[i]->GetPlayer().GetScore();
            if (population[i]->GetPlayer().GetScore() < population[i + 1]->GetPlayer().GetScore()) {
                Game *tmp = population[i];
                population[i] = population[i + 1];
                population[i + 1] = tmp;
                ok = false;
            }    
        }
    }
    //bestScoreText->SetText(std::string("Best score: ") + std::to_string(maxScore));
    ReCalcEnvironmentsPosition();
}

void GAMonitor::ReCalcEnvironmentsPosition() {
    const int minWidth_ = GetEngine()->GetWindowWidth() / Project()->ais.size();
    const int envWidthTmp = minWidth_ < MIN_ENV_WIDTH ? MIN_ENV_WIDTH : minWidth_;
    const int envPerRow = GetEngine()->GetWindowWidth() / envWidthTmp;

    const int envWidth = GetEngine()->GetWindowWidth() / envPerRow;
    const int envHeight = envWidth;

    for (int i = 0; i < Project()->ais.size(); ++i) {
        Project()->ais[i]->SetWidth(envWidth);
        Project()->ais[i]->SetHeight(envHeight);
        Project()->ais[i]->SetPosition((envWidth + 1) * (i % envPerRow), (i / envPerRow) * (envHeight + 1));
    }
}

void GAMonitor::ReproducePopulation(float elitePrct, float randomPrct) {
    int nbElite = Project()->ais.size() * elitePrct;
    int nbEliteParents = sqrt(nbElite);
    int nbEliteTotal = nbElite + nbEliteParents;

    int nbRandom = Project()->ais.size() * randomPrct;
    int nbRandomParents = sqrt(nbRandom);
    int nbRandomTotal = nbRandom;

    if (nbEliteTotal + nbRandomTotal > Project()->ais.size())
    {
        std::cout << "LOL (this should never happen)" << std::endl;
        exit(1);
    }

    int k = nbEliteParents;
    for (int i = 0; i < nbEliteParents; ++i) {
        for (int j = 0; j < nbEliteParents; ++j) {
            if (i == j)
                continue;
            LearNN::NeuralNetwork &father = Project()->ais[i]->GetAI()->GetNeuralNetwork();
            LearNN::NeuralNetwork &mother = Project()->ais[j]->GetAI()->GetNeuralNetwork();
            LearNN::NeuralNetwork &child = Project()->ais[k]->GetAI()->GetNeuralNetwork();

            for (int layerIndex = 0; layerIndex < child.GetLayersPointer().size(); ++layerIndex) {
                for (int weightIndex = 0; weightIndex < child.GetLayersPointer()[layerIndex]->GetWeightsPointer().size(); ++weightIndex) {
                    if ((double)rand() / RAND_MAX < 0.5)
                        child.GetLayersPointer()[layerIndex]->GetWeightsPointer()[weightIndex] = father.GetLayersPointer()[layerIndex]->GetWeightsPointer()[weightIndex];
                    else
                        child.GetLayersPointer()[layerIndex]->GetWeightsPointer()[weightIndex] = mother.GetLayersPointer()[layerIndex]->GetWeightsPointer()[weightIndex];
                    
                    if ((double)rand() / RAND_MAX < 0.05) {
                        child.GetLayersPointer()[layerIndex]->GetWeightsPointer()[weightIndex] = (((2 * (double)rand()) / (double)RAND_MAX) - 1);
                    }
                }
            }
            ++k;
        }
    }
    
    k = nbEliteTotal;
    for (int i = 0; i < nbRandomParents; ++i) {
        for (int j = 0; j < nbRandomParents; ++j) {
            LearNN::NeuralNetwork &father = Project()->ais[rand() % Project()->ais.size()]->GetAI()->GetNeuralNetwork();
            LearNN::NeuralNetwork &mother = Project()->ais[rand() % Project()->ais.size()]->GetAI()->GetNeuralNetwork();
            if (&father == &mother)
                continue;
            LearNN::NeuralNetwork &child = Project()->ais[k]->GetAI()->GetNeuralNetwork();
            for (int layerIndex = 0; layerIndex < child.GetLayersPointer().size(); ++layerIndex) {
                for (int weightIndex = 0; weightIndex < child.GetLayersPointer()[layerIndex]->GetWeightsPointer().size(); ++weightIndex) {
                    if ((double)rand() / RAND_MAX < 0.5)
                        child.GetLayersPointer()[layerIndex]->GetWeightsPointer()[weightIndex] = father.GetLayersPointer()[layerIndex]->GetWeightsPointer()[weightIndex];
                    else
                        child.GetLayersPointer()[layerIndex]->GetWeightsPointer()[weightIndex] = mother.GetLayersPointer()[layerIndex]->GetWeightsPointer()[weightIndex];
                    
                    if ((double)rand() / RAND_MAX < 0.05) {
                        child.GetLayersPointer()[layerIndex]->GetWeightsPointer()[weightIndex] = (((2 * (double)rand()) / (double)RAND_MAX) - 1);
                    }
                }
            }
            ++k;
        }
    }
    
    for (int i = nbEliteTotal + nbRandomTotal; i < Project()->ais.size(); ++i) {
        LearNN::NeuralNetwork &child = Project()->ais[i]->GetAI()->GetNeuralNetwork();
        for (int layerIndex = 0; layerIndex < child.GetLayersPointer().size(); ++layerIndex) {
            for (int weightIndex = 0; weightIndex < child.GetLayersPointer()[layerIndex]->GetWeightsPointer().size(); ++weightIndex) {
                child.GetLayersPointer()[layerIndex]->GetWeightsPointer()[weightIndex] = (((2 * (double)rand()) / (double)RAND_MAX) - 1);
            }
        }
    }
}