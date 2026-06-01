#pragma once
#include <SFML/Graphics.hpp>
#include <string>

const int WINDOW_W = 1280;
const int WINDOW_H = 720;

const sf::Color COL_BG_TOP      = sf::Color(15,  25,  45);
const sf::Color COL_BG_BOT      = sf::Color(5,   10,  20);
const sf::Color COL_WHITE       = sf::Color(255, 255, 255);
const sf::Color COL_GRAY        = sf::Color(180, 180, 200);
const sf::Color COL_ACCENT      = sf::Color(0,   230, 180);
const sf::Color COL_DANGER      = sf::Color(220, 60,  50);
const sf::Color COL_HP_FULL     = sf::Color(80,  255, 80);
const sf::Color COL_HP_LOW      = sf::Color(255, 80,  40);
const sf::Color COL_TARGET_NEAR = sf::Color(255, 80,  40);
const sf::Color COL_TARGET_FAR  = sf::Color(120, 30,  10);
const sf::Color COL_PENALTY = sf::Color(220, 80, 40);

const std::string FONT_PATH = "font.ttf";
const std::string SETTINGS_PATH = "settings.ini";

//игра
const float GAME_DURATION      = 60.f;
const float TARGET_RADIUS_MIN  = 20.f;
const float TARGET_RADIUS_MAX  = 60.f;
const float TARGET_HP_BAR_H    = 6.f;

//лимиты целей
const int LIMIT_STATIC   = 1;
const int LIMIT_MOVING   = 2;
const int LIMIT_SHRINK   = 3;

enum class GameMode {
    STATIC,
    MOVING,
    SHRINKING
};
enum class CrosshairStyle { 
    CLASSIC, 
    DOT, 
    CROSS, 
    CIRCLE 
};

//настройки игры
struct Settings {
    CrosshairStyle crosshairStyle = CrosshairStyle::CLASSIC;
    sf::Color      crosshairColor = sf::Color(0, 230, 180);
    float          crosshairSize  = 1.0f;   // множитель 0.5 .. 2.0
    float          masterVolume   = 1.0f;   // 0..1 (пока заглушка)
    bool           showFps        = false;

    void save() const;
    void load();
};