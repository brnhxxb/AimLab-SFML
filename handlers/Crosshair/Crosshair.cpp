#include "Crosshair.h"
#include <cmath>
#include <algorithm>

void Crosshair::shoot() { pulse = 1.f; }

void Crosshair::update(sf::Vector2f mousePos, float dt) {
    pos   = mousePos;
    pulse -= dt * 8.f;
    if (pulse < 0.f) pulse = 0.f;
    gap = 10.f - 7.f * pulse;
}

void Crosshair::drawLine(sf::RenderWindow& w, sf::Vector2f a, sf::Vector2f b,
                         sf::Color col, float thick) const {
    sf::Vector2f d = b - a;
    float len = std::sqrt(d.x*d.x + d.y*d.y);
    if (len < 0.001f) return;
    sf::RectangleShape line({len, thick});
    line.setOrigin({0.f, thick/2.f});
    line.setPosition(a);
    line.setRotation(sf::degrees(std::atan2(d.y, d.x) * 180.f / 3.14159265f));
    line.setFillColor(col);
    w.draw(line);
}

void Crosshair::draw(sf::RenderWindow& window, const Settings& s) const {
    sf::Color col = s.crosshairColor;
    float sz   = s.crosshairSize;
    float len  = 14.f * sz;
    float g    = gap * sz;
    float thick = 2.f * sz;

    switch (s.crosshairStyle) {

    case CrosshairStyle::CLASSIC:
        drawLine(window, {pos.x - g - len, pos.y}, {pos.x - g,       pos.y}, col, thick);
        drawLine(window, {pos.x + g,       pos.y}, {pos.x + g + len,  pos.y}, col, thick);
        drawLine(window, {pos.x, pos.y - g - len}, {pos.x, pos.y - g      }, col, thick);
        drawLine(window, {pos.x, pos.y + g      }, {pos.x, pos.y + g + len}, col, thick);
        {   // центральная точка
            sf::CircleShape dot(2.5f * sz);
            dot.setOrigin({2.5f*sz, 2.5f*sz});
            dot.setPosition(pos);
            dot.setFillColor({255, 255, 255, 200});
            window.draw(dot);
        }
        break;

    case CrosshairStyle::DOT:
        {
            sf::CircleShape dot(5.f * sz);
            dot.setOrigin({5.f*sz, 5.f*sz});
            dot.setPosition(pos);
            dot.setFillColor(col);
            window.draw(dot);
        }
        break;

    case CrosshairStyle::CROSS:
        // полный крест без зазора
        drawLine(window, {pos.x - len, pos.y}, {pos.x + len, pos.y}, col, thick);
        drawLine(window, {pos.x, pos.y - len}, {pos.x, pos.y + len}, col, thick);
        break;

    case CrosshairStyle::CIRCLE:
        {
            float r = (g + len * 0.5f);
            sf::CircleShape ring(r);
            ring.setOrigin({r, r});
            ring.setPosition(pos);
            ring.setFillColor(sf::Color::Transparent);
            ring.setOutlineThickness(thick);
            ring.setOutlineColor(col);
            window.draw(ring);
            // центр
            sf::CircleShape dot(2.5f * sz);
            dot.setOrigin({2.5f*sz, 2.5f*sz});
            dot.setPosition(pos);
            dot.setFillColor(col);
            window.draw(dot);
        }
        break;
    }
}
