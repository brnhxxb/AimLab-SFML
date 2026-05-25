#pragma once
#include <SFML/Graphics.hpp>
#include "../Constants/Constants.h"
#include "../Game/Game.h"

class ResultScreen {
public:
    ResultScreen(sf::RenderWindow& window, sf::Font& font, const GameStats& stats);

    int  update(float dt);          // 0=снова, 1=меню, -1=ждём
    void handleEvent(const sf::Event& event);
    void draw();

private:
    sf::RenderWindow& window;
    sf::Font&         font;
    GameStats stats;
    float animTime;
    int   hovered;   // 0=again, 1=menu
    int   action;    // -1=none, 0=again, 1=menu
};
