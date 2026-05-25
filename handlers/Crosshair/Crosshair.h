#pragma once
#include <SFML/Graphics.hpp>

class Crosshair {
public:
    void shoot();
    void update(sf::Vector2f mousePos, float dt);
    void draw(sf::RenderWindow& window);

private:
    sf::Vector2f pos;
    float pulse = 0.f;
    float gap   = 10.f;
};
