#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "../Constants/Constants.h"

class Menu {
public:
    Menu(sf::RenderWindow& window, sf::Font& font);

    //>= 0: выбран режим (0/1/2)
    //-2:   открыть настройки
    //-1:   ничего
    int  update(float dt);
    void handleEvent(const sf::Event& event);
    void draw();

private:
    sf::RenderWindow& window;
    sf::Font&         font;
    int   hovered  = -1;
    float animTime = 0.f;
    int   selected = -1;

    struct ModeCard {
        std::string title;
        std::string desc;
        std::string detail;
        sf::Color   color;
    };
    std::vector<ModeCard> cards;
};
