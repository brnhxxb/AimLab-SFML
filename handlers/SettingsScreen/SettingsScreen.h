#pragma once
#include <SFML/Graphics.hpp>
#include "../Constants/Constants.h"

// Экран настроек
class SettingsScreen {
public:
    SettingsScreen(sf::RenderWindow& window, sf::Font& font, Settings& settings);

    //-1 = ещё открыт, 0 = закрыть и вернуться в меню
    int  update(float dt);
    void handleEvent(const sf::Event& event);
    void draw();

private:
    sf::RenderWindow& window;
    sf::Font&         font;
    Settings&         settings;

    float animTime = 0.f;
    int   action   = -1;

    //позунки и кнопки
    int   hoveredStyle = -1;
    int   hoveredColor = -1;
    float sizeSliderX  = 0.f;   // позиция перетаскивания
    bool  dragging     = false;

    struct ColorPreset { sf::Color col; std::string name; };
    std::vector<ColorPreset> colorPresets;

    void drawBackground();
    void drawStyleButtons();
    void drawColorPicker();
    void drawSizeSlider();
    void drawPreview();
};
