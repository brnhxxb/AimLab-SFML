#pragma once
#include <SFML/Graphics.hpp>
#include "../Constants/Constants.h"

class Crosshair {
public:
    void shoot();
    void update(sf::Vector2f mousePos, float dt);
    void draw(sf::RenderWindow& window, const Settings& s) const;

private:
    sf::Vector2f pos;
    float pulse = 0.f;
    float gap   = 10.f;

    void drawLine(sf::RenderWindow& w, sf::Vector2f a, sf::Vector2f b,
                  sf::Color col, float thick = 2.f) const;
};
