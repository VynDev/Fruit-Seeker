#include <SFML/Graphics.hpp>
#include "Fruit.h"
#include "EnvGine/Surface.h"
#include "EnvGine/Texture.h"


Fruit::Fruit(Environment *environment) : Object(environment) {

    Texture *texture = new Texture(128, 128);

    texture->GetSFMLRenderTexture().clear(sf::Color(0, 200, 0));
    SetTexture(texture);
}