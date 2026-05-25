#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "../Constants/Constants.h"

class Menu {
public:
    Menu(sf::RenderWindow& window, sf::Font& font);
    int  update(float dt);
    void handleEvent(const sf::Event& event);
    void draw();

private:
    sf::RenderWindow& window;
    sf::Font&         font;
    int   hovered;
    float animTime;
    int   selected;

    struct ModeCard {
        std::string title;
        std::string desc;
        sf::Color   color;
    };
    std::vector<ModeCard> cards;
};
