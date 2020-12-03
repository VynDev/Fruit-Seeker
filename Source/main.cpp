#include "EnvGine/Engine.h"
#include "Project.h"
#include "Menu.h"

using namespace std;

ProjectEnvironments *Project() {
    static ProjectEnvironments *envs = nullptr;
    if (envs == nullptr)
        envs = new ProjectEnvironments;
    return envs;
}

int main(void) {
    Engine *engine = Engine::Init(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);
    Project()->menu = engine->CreateEnvironment<Menu>();
    engine->Execute(Project()->menu);
    return 0;
}