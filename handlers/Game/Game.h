#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "../Constants/Constants.h"
#include "../Target/Target.h"
#include "../Crosshair/Crosshair.h"
#include "../Particle/ParticleSystem.h"

struct GameStats {
    int   score      = 0;
    int   shots      = 0;
    int   hits       = 0;
    int   misses     = 0;
    float bestReact  = 99.f;
    float totalReact = 0.f;
    int   reactCount = 0;
    GameMode mode    = GameMode::STATIC;

    float accuracy() const { return shots > 0 ? (100.f * hits / shots) : 0.f; }
    float avgReact()  const { return reactCount > 0 ? (totalReact / reactCount * 1000.f) : 0.f; }
};

class Game {
public:
    Game(sf::RenderWindow& window, sf::Font& font, GameMode mode);

    bool update(float dt);                    // false = время вышло
    void handleEvent(const sf::Event& event);
    void draw();

    const GameStats& getStats() const { return stats; }

private:
    sf::RenderWindow& window;
    sf::Font&         font;
    GameMode          mode;

    std::vector<std::unique_ptr<Target>> targets;
    Crosshair      crosshair;
    ParticleSystem particles;
    GameStats      stats;

    float timeLeft;
    float spawnTimer;
    float spawnInterval;
    float reactionClock;

    void drawBackground();
    void drawHUD();
    void spawnTarget();
    void spawnBurst(int count);

    struct FloatText {
        sf::Vector2f pos;
        std::string  text;
        float        life;
        sf::Color    color;
    };
    std::vector<FloatText> floatTexts;
};
