#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <deque>

#include "../Constants/Constants.h"
#include "../Target/Target.h"
#include "../Crosshair/Crosshair.h"
#include "../Particle/ParticleSystem.h"

struct GameStats {
    int   score       = 0;
    int   shots       = 0;
    int   hits        = 0;
    int   misses      = 0;
    int   penaltyPts  = 0;      // суммарный штраф
    float bestReact   = 99.f;
    float totalReact  = 0.f;
    int   reactCount  = 0;
    GameMode mode     = GameMode::STATIC;

    float accuracy() const { return shots > 0 ? (100.f * hits / shots) : 0.f; }
    float avgReact()  const { return reactCount > 0 ? (totalReact / reactCount * 1000.f) : 0.f; }
};

class Game {
public:
    Game(sf::RenderWindow& window, sf::Font& font, GameMode mode,
         const Settings& settings, const GameStats* prevStats = nullptr);

    bool update(float dt);
    void handleEvent(const sf::Event& event);
    void draw();

    const GameStats& getStats() const { return stats; }

private:
    float penaltyAccum = 0.f;
    sf::RenderWindow& window;
    sf::Font&         font;
    GameMode          mode;
    const Settings&   settings;

    std::vector<std::unique_ptr<Target>> targets;
    Crosshair      crosshair;
    ParticleSystem particles;
    GameStats      stats;

    float timeLeft;
    float spawnTimer;
    float spawnInterval;
    float reactionClock;

    //адаптивный штраф
    float  avgKillTime   = 0.f;   // из предыдущей игры (0 = нет данных)
    float  penaltyTimer  = 0.f;   // сколько секунд цель "живёт лишнее"
    bool   penaltyActive = false;

    int maxTargets() const;       // лимит по режиму

    void drawBackground();
    void drawHUD();
    void spawnTarget();

    struct FloatText {
        sf::Vector2f pos;
        std::string  text;
        float        life;
        sf::Color    color;
    };
    std::vector<FloatText> floatTexts;
};
