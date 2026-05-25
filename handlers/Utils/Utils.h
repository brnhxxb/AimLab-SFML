#pragma once
#include <SFML/Graphics.hpp>

namespace Utils {
    void drawLine(sf::RenderWindow& w,
                  sf::Vector2f a, sf::Vector2f b,
                  sf::Color col, float thick);

    void drawGradientBg(sf::RenderWindow& w, sf::Color top, sf::Color bot);
    void drawGrid(sf::RenderWindow& w);
}
