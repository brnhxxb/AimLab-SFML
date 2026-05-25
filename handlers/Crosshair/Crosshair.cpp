#include "Crosshair.h"
#include <cmath>

void Crosshair::shoot() {
    pulse = 1.f;
}

void Crosshair::update(sf::Vector2f mousePos, float dt) {
    pos   = mousePos;
    pulse -= dt * 8.f;
    if (pulse < 0.f) pulse = 0.f;
    gap = 10.f - 7.f * pulse;
}

void Crosshair::draw(sf::RenderWindow& window) {
    const float     len   = 14.f;
    const float     thick = 2.f;
    const sf::Color col(0, 230, 180, 220);
    const sf::Color dot(255, 255, 255, 200);

    // SFML 3: setRotation принимает sf::Angle, используем sf::degrees()
    auto drawLine = [&](sf::Vector2f a, sf::Vector2f b) {
        sf::Vector2f d = b - a;
        float length = std::sqrt(d.x * d.x + d.y * d.y);

        sf::RectangleShape line({length, thick});
        line.setOrigin({0.f, thick / 2.f});
        line.setPosition(a);
        line.setRotation(sf::degrees(std::atan2(d.y, d.x) * 180.f / 3.14159265f));
        line.setFillColor(col);
        window.draw(line);
    };

    drawLine({pos.x - gap - len, pos.y}, {pos.x - gap,       pos.y});
    drawLine({pos.x + gap,       pos.y}, {pos.x + gap + len,  pos.y});
    drawLine({pos.x, pos.y - gap - len}, {pos.x, pos.y - gap      });
    drawLine({pos.x, pos.y + gap      }, {pos.x, pos.y + gap + len});

    sf::CircleShape d(2.5f);
    d.setOrigin({2.5f, 2.5f});
    d.setPosition(pos);
    d.setFillColor(dot);
    window.draw(d);
}
