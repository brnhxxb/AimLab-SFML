#include "Game.h"
#include <cstdlib>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <algorithm>

static float rF(float lo, float hi) {
    return lo + (hi - lo) * (std::rand() / (float)RAND_MAX);
}

Game::Game(sf::RenderWindow& window, sf::Font& font, GameMode mode)
    : window(window), font(font), mode(mode),
      timeLeft(GAME_DURATION), spawnTimer(0.f), reactionClock(0.f)
{
    stats.mode = mode;
    switch (mode) {
        case GameMode::STATIC:    spawnInterval = 1.4f; break;
        case GameMode::MOVING:    spawnInterval = 1.0f; break;
        case GameMode::SHRINKING: spawnInterval = 0.9f; break;
    }
    spawnBurst(3);
}

void Game::spawnTarget() {
    float margin = 80.f;
    float x     = rF(margin, (float)WINDOW_W - margin);
    float y     = rF(100.f + margin, (float)WINDOW_H - margin);
    float depth = rF(0.f, 1.f);
    targets.push_back(std::make_unique<Target>(sf::Vector2f(x, y), depth, mode));
    reactionClock = 0.f;
}

void Game::spawnBurst(int count) {
    for (int i = 0; i < count; ++i) spawnTarget();
}

bool Game::update(float dt) {
    timeLeft      -= dt;
    reactionClock += dt;
    spawnTimer    += dt;

    int alive = 0;
    for (auto& t : targets) if (t->isAlive()) ++alive;

    if (spawnTimer >= spawnInterval || alive == 0) {
        spawnTimer = 0.f;
        if (mode == GameMode::MOVING) spawnBurst(2);
        else                          spawnTarget();
    }

    for (auto& t : targets) t->update(dt);

    targets.erase(
        std::remove_if(targets.begin(), targets.end(),
            [](const std::unique_ptr<Target>& t){ return t->isDead(); }),
        targets.end()
    );

    particles.update(dt);

    for (auto& ft : floatTexts) ft.life -= dt;
    floatTexts.erase(
        std::remove_if(floatTexts.begin(), floatTexts.end(),
            [](const FloatText& ft){ return ft.life <= 0.f; }),
        floatTexts.end()
    );

    return timeLeft > 0.f;
}

// ---------- SFML 3: обработка событий через std::visit ----------
void Game::handleEvent(const sf::Event& event) {
    // Клик мышью
    if (const auto* e = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (e->button == sf::Mouse::Button::Left) {
            sf::Vector2f click((float)e->position.x, (float)e->position.y);
            stats.shots++;
            crosshair.shoot();

            bool hitAny = false;
            for (auto& t : targets) {
                if (t->isAlive() && t->contains(click)) {
                    bool killed = t->hit(34.f);
                    hitAny = true;
                    stats.hits++;

                    if (killed) {
                        float sizeBonus  = (1.f - (t->getRadius() - TARGET_RADIUS_MIN) /
                                               (TARGET_RADIUS_MAX - TARGET_RADIUS_MIN));
                        float reactBonus = std::max(0.f, 1.f - reactionClock / 2.f);
                        int   pts        = (int)(100 + sizeBonus * 50 + reactBonus * 50);
                        stats.score     += pts;

                        stats.totalReact += reactionClock;
                        stats.reactCount++;
                        if (reactionClock < stats.bestReact) stats.bestReact = reactionClock;
                        reactionClock = 0.f;

                        particles.emit(t->getPosition(), COL_TARGET_NEAR, 18);
                        floatTexts.push_back({t->getPosition(),
                            "+" + std::to_string(pts), 0.8f, COL_ACCENT});
                    } else {
                        floatTexts.push_back({click, "HIT", 0.4f,
                            sf::Color(255, 200, 50)});
                    }
                    break;
                }
            }

            if (!hitAny) {
                stats.misses++;
                floatTexts.push_back({click, "MISS", 0.5f, COL_DANGER});
            }
        }
    }

    // Движение мыши
    if (const auto* e = event.getIf<sf::Event::MouseMoved>()) {
        crosshair.update(sf::Vector2f((float)e->position.x, (float)e->position.y), 0.f);
    }
}

// ---------- Фон ----------
void Game::drawBackground() {
    // SFML 3: sf::Vertex инициализируется через {.position=, .color=}
    sf::VertexArray bg(sf::PrimitiveType::TriangleStrip, 4);
    bg[0] = sf::Vertex({sf::Vector2f(0.f,          0.f),          COL_BG_TOP});
    bg[1] = sf::Vertex({sf::Vector2f((float)WINDOW_W, 0.f),          COL_BG_TOP});
    bg[2] = sf::Vertex({sf::Vector2f(0.f,          (float)WINDOW_H), COL_BG_BOT});
    bg[3] = sf::Vertex({sf::Vector2f((float)WINDOW_W, (float)WINDOW_H), COL_BG_BOT});
    window.draw(bg);

    float     horizon = (float)WINDOW_H * 0.55f;
    sf::Color gridCol(30, 50, 90, 60);

    for (int i = 0; i <= 10; ++i) {
        float tx = (float)WINDOW_W * i / 10.f;
        float hx = (float)WINDOW_W / 2.f + (tx - (float)WINDOW_W / 2.f) * 0.3f;
        sf::VertexArray line(sf::PrimitiveType::Lines, 2);
        line[0] = sf::Vertex({sf::Vector2f(tx, (float)WINDOW_H), gridCol});
        line[1] = sf::Vertex({sf::Vector2f(hx, horizon),          gridCol});
        window.draw(line);
    }

    for (int i = 0; i <= 8; ++i) {
        float t = (float)i / 8.f;
        float y = horizon + ((float)WINDOW_H - horizon) * t * t;
        sf::VertexArray line(sf::PrimitiveType::Lines, 2);
        line[0] = sf::Vertex({sf::Vector2f(0.f,          y), gridCol});
        line[1] = sf::Vertex({sf::Vector2f((float)WINDOW_W, y), gridCol});
        window.draw(line);
    }
}

// ---------- HUD ----------
void Game::drawHUD() {
    sf::RectangleShape bar({(float)WINDOW_W, 64.f});
    bar.setFillColor(sf::Color(0, 0, 0, 120));
    window.draw(bar);

    auto txt = [&](const std::string& s, float x, float y,
                   unsigned size, sf::Color col)
    {
        sf::Text t(font, s, size);
        t.setFillColor(col);
        t.setPosition({x, y});
        window.draw(t);
    };

    int      secs     = (int)std::ceil(timeLeft);
    sf::Color timerCol = (timeLeft < 10.f) ? COL_DANGER : COL_WHITE;
    txt(std::to_string(secs) + "s", 20.f, 10.f, 36u, timerCol);

    txt("SCORE  " + std::to_string(stats.score),
        (float)WINDOW_W / 2.f - 100.f, 12.f, 30u, COL_ACCENT);

    std::ostringstream acc;
    acc << std::fixed << std::setprecision(1) << stats.accuracy() << "% ACC";
    txt(acc.str(), (float)WINDOW_W - 200.f, 12.f, 24u, COL_GRAY);

    std::string modeStr;
    switch (mode) {
        case GameMode::STATIC:    modeStr = "STATIC"; break;
        case GameMode::MOVING:    modeStr = "MOVING"; break;
        case GameMode::SHRINKING: modeStr = "SHRINK"; break;
    }
    txt(modeStr, (float)WINDOW_W - 200.f, 38.f, 18u, COL_GRAY);
}

// ---------- Draw ----------
void Game::draw() {
    drawBackground();

    std::vector<Target*> sorted;
    for (auto& t : targets) sorted.push_back(t.get());
    std::sort(sorted.begin(), sorted.end(),
        [](Target* a, Target* b){ return a->getDepth() > b->getDepth(); });
    for (auto* t : sorted) t->draw(window);

    particles.draw(window);

    for (const auto& ft : floatTexts) {
        sf::Text t(font, ft.text, 22u);
        sf::Color c = ft.color;
        c.a = static_cast<uint8_t>(255.f * (ft.life / 0.8f));
        t.setFillColor(c);
        float rise = (0.8f - ft.life) * 40.f;
        t.setPosition({ft.pos.x - 20.f, ft.pos.y - 30.f - rise});
        window.draw(t);
    }

    drawHUD();
    crosshair.draw(window);
}
