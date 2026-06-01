#include "ResultScreen.h"
#include <sstream>
#include <iomanip>
#include <cmath>
#include <cstdint>
#include <algorithm>

ResultScreen::ResultScreen(sf::RenderWindow& window, sf::Font& font, const GameStats& stats)
    : window(window), font(font), stats(stats), animTime(0.f), hovered(-1), action(-1)
{}

int ResultScreen::update(float dt) {
    animTime += dt;
    int ret = action;
    action  = -1;
    return ret;
}

void ResultScreen::handleEvent(const sf::Event& event) {
    const float by     = (float)WINDOW_H - 120.f;
    const float halfW  = (float)WINDOW_W / 2.f;

    if (const auto* e = event.getIf<sf::Event::MouseMoved>()) {
        sf::Vector2f m((float)e->position.x, (float)e->position.y);
        hovered = -1;
        if (m.x >= halfW - 130.f && m.x <= halfW + 110.f && m.y >= by && m.y <= by + 50.f)
            hovered = 0;
        if (m.x >= halfW + 130.f && m.x <= halfW + 290.f && m.y >= by && m.y <= by + 50.f)
            hovered = 1;
    }

    if (const auto* e = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (e->button == sf::Mouse::Button::Left) {
            if (hovered == 0) action = 0;
            if (hovered == 1) action = 1;
        }
    }

    // SFML 3: sf::Keyboard::Key::Enter / Escape
    if (const auto* e = event.getIf<sf::Event::KeyPressed>()) {
        if (e->code == sf::Keyboard::Key::Enter)  action = 0;
        if (e->code == sf::Keyboard::Key::Escape) action = 1;
    }
}

void ResultScreen::draw() {
    // Фон
    sf::VertexArray bg(sf::PrimitiveType::TriangleStrip, 4);
    bg[0] = sf::Vertex({sf::Vector2f(0.f,          0.f),          COL_BG_TOP});
    bg[1] = sf::Vertex({sf::Vector2f((float)WINDOW_W, 0.f),          COL_BG_TOP});
    bg[2] = sf::Vertex({sf::Vector2f(0.f,          (float)WINDOW_H), COL_BG_BOT});
    bg[3] = sf::Vertex({sf::Vector2f((float)WINDOW_W, (float)WINDOW_H), COL_BG_BOT});
    window.draw(bg);

    // Хелпер: центрированный текст
    auto drawCentered = [&](const std::string& s, float y, unsigned sz, sf::Color col) {
        sf::Text t(font, s, sz);
        t.setFillColor(col);
        sf::FloatRect r = t.getLocalBounds();
        t.setOrigin({r.size.x / 2.f, r.size.y / 2.f});
        t.setPosition({(float)WINDOW_W / 2.f, y});
        window.draw(t);
    };

    drawCentered("РЕЗУЛЬТАТЫ", 80.f, 60u, COL_WHITE);

    std::string modeStr;
    switch (stats.mode) {
        case GameMode::STATIC:    modeStr = "STATIC"; break;
        case GameMode::MOVING:    modeStr = "MOVING"; break;
        case GameMode::SHRINKING: modeStr = "SHRINK"; break;
    }
    drawCentered("Режим: " + modeStr, 145.f, 22u, COL_GRAY);

    // Таблица результатов
    struct Row { std::string label; std::string value; sf::Color vc; };
    std::ostringstream accStr, reactStr, bestStr;
    accStr   << std::fixed << std::setprecision(1) << stats.accuracy()         << "%";
    reactStr << std::fixed << std::setprecision(0) << stats.avgReact()          << " ms";
    bestStr  << std::fixed << std::setprecision(0) << stats.bestReact * 1000.f  << " ms";

    std::vector<Row> rows = {
        {"Очки",           std::to_string(stats.score),   COL_ACCENT              },
        {"Точность",       accStr.str(),                   sf::Color(100,200,255)  },
        {"Выстрелов",      std::to_string(stats.shots),   COL_WHITE               },
        {"Попаданий",      std::to_string(stats.hits),    COL_HP_FULL             },
        {"Промахов",      std::to_string(stats.misses),  COL_DANGER              },
        {"Штраф очки",   "-"+std::to_string(stats.penaltyPts), COL_PENALTY             },
        {"Средняя реакция",reactStr.str(),                 sf::Color(255,200,80)   },
        {"Лучшая реакция", bestStr.str(),                  COL_ACCENT              },
    };

    const float startY = 210.f, rowH = 52.f;
    const float panelW = 500.f;
    const float panelX = ((float)WINDOW_W - panelW) / 2.f;

    for (int i = 0; i < (int)rows.size(); ++i) {
        float y     = startY + i * rowH;
        float alpha = std::min(1.f, animTime * 3.f - i * 0.15f);
        if (alpha <= 0.f) continue;

        sf::RectangleShape rowBg({panelW, rowH - 4.f});
        rowBg.setPosition({panelX, y});
        rowBg.setFillColor((i % 2 == 0) ? sf::Color(255,255,255,12) : sf::Color(0,0,0,0));
        window.draw(rowBg);

        {
            sf::Text lbl(font, rows[i].label, 22u);
            lbl.setFillColor(COL_GRAY);
            lbl.setPosition({panelX + 20.f, y + 10.f});
            window.draw(lbl);
        }
        {
            sf::Text val(font, rows[i].value, 26u);
            sf::Color vc = rows[i].vc;
            vc.a = static_cast<uint8_t>(255 * alpha);
            val.setFillColor(vc);
            sf::FloatRect vr = val.getLocalBounds();
            val.setPosition({panelX + panelW - vr.size.x - 20.f, y + 8.f});
            window.draw(val);
        }
    }

    // Кнопки
    const float by    = (float)WINDOW_H - 120.f;
    const float halfW = (float)WINDOW_W / 2.f;

    auto drawBtn = [&](const std::string& s, float x, float w, bool hov, sf::Color col) {
        sf::RectangleShape btn({w, 50.f});
        btn.setPosition({x, by});
        btn.setFillColor(sf::Color(
            static_cast<uint8_t>(col.r / 6),
            static_cast<uint8_t>(col.g / 6),
            static_cast<uint8_t>(col.b / 6),
            static_cast<uint8_t>(hov ? 200 : 120)
        ));
        sf::Color bc = col;
        bc.a = static_cast<uint8_t>(hov ? 255 : 160);
        btn.setOutlineColor(bc);
        btn.setOutlineThickness(hov ? 2.5f : 1.5f);
        window.draw(btn);

        sf::Text t(font, s, 22u);
        t.setFillColor(bc);
        sf::FloatRect r = t.getLocalBounds();
        t.setOrigin({r.size.x / 2.f, r.size.y / 2.f});
        t.setPosition({x + w / 2.f, by + 25.f});
        window.draw(t);
    };

    drawBtn("ИГРАТЬ СНОВА", halfW - 130.f, 240.f, hovered == 0, COL_ACCENT);
    drawBtn("МЕНЮ",         halfW + 130.f, 160.f, hovered == 1, COL_GRAY);
}
