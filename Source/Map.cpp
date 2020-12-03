#include <SFML/Graphics.hpp>
#include "Map.h"
#include "EnvGine/Surface.h"
#include "EnvGine/Texture.h"
#include <iostream>

Map::Map(Environment *environment) : Object(environment) {

    int w = 512;
    int h = w;

    Texture *texture = new Texture(w, h);
    texture->GetSFMLRenderTexture().clear(sf::Color(0, 0, 0));

    sf::RectangleShape horizontalBorderShape(sf::Vector2f(w, 4));
    horizontalBorderShape.setFillColor(sf::Color(255, 0, 0));

    sf::RectangleShape verticalBorderShape(sf::Vector2f(4, h));
    verticalBorderShape.setFillColor(sf::Color(255, 0, 0));

    horizontalBorderShape.setPosition(0, 0);
    texture->GetSFMLRenderTexture().draw(horizontalBorderShape);
    horizontalBorderShape.setPosition(0, w - 4);
    texture->GetSFMLRenderTexture().draw(horizontalBorderShape);

    verticalBorderShape.setPosition(0, 0);
    texture->GetSFMLRenderTexture().draw(verticalBorderShape);
    verticalBorderShape.setPosition(h - 4, 0);
    texture->GetSFMLRenderTexture().draw(verticalBorderShape);

    texture->GetSFMLRenderTexture().display();

    SetTexture(texture);
}

void Map::SetWidthCaseCount(int value) {
    widthCaseCount = value;
}

void Map::SetHeightCaseCount(int value) {
    heightCaseCount = value;
}

int Map::GetWidthCaseCount() const {
    return widthCaseCount;
}

int Map::GetHeightCaseCount() const {
    return heightCaseCount;
}

int Map::GetCaseWidth() const {
    return GetWidth() / GetWidthCaseCount();
}

int Map::GetCaseHeight() const {
    return GetHeight() / GetHeightCaseCount();
}