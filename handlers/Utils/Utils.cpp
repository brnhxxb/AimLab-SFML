#include "Utils.h"
#include "../Constants/Constants.h"
#include <cmath>

namespace Utils {

void drawLine(sf::RenderWindow& w,
              sf::Vector2f a, sf::Vector2f b,
              sf::Color col, float thick)
{
    sf::Vector2f d = b - a;
    float len = std::sqrt(d.x * d.x + d.y * d.y);
    if (len < 0.001f) return;

    sf::RectangleShape line({len, thick});
    line.setOrigin({0.f, thick / 2.f});
    line.setPosition(a);
    // SFML 3: setRotation принимает sf::Angle
    line.setRotation(sf::degrees(std::atan2(d.y, d.x) * 180.f / 3.14159265f));
    line.setFillColor(col);
    w.draw(line);
}

void drawGradientBg(sf::RenderWindow& w, sf::Color top, sf::Color bot) {
    sf::VertexArray bg(sf::PrimitiveType::TriangleStrip, 4);
    bg[0] = sf::Vertex({sf::Vector2f(0.f,          0.f),          top});
    bg[1] = sf::Vertex({sf::Vector2f((float)WINDOW_W, 0.f),          top});
    bg[2] = sf::Vertex({sf::Vector2f(0.f,          (float)WINDOW_H), bot});
    bg[3] = sf::Vertex({sf::Vector2f((float)WINDOW_W, (float)WINDOW_H), bot});
    w.draw(bg);
}

void drawGrid(sf::RenderWindow& w) {
    const sf::Color gc(30, 50, 90, 55);
    const float horizon = (float)WINDOW_H * 0.55f;
    const float cx      = (float)WINDOW_W / 2.f;

    for (int i = 0; i <= 10; ++i) {
        float bx = (float)WINDOW_W * i / 10.f;
        float hx = cx + (bx - cx) * 0.3f;
        drawLine(w, {bx, (float)WINDOW_H}, {hx, horizon}, gc, 1.f);
    }

    for (int i = 0; i <= 8; ++i) {
        float t = (float)i / 8.f;
        float y = horizon + ((float)WINDOW_H - horizon) * t * t;
        drawLine(w, {0.f, y}, {(float)WINDOW_W, y}, gc, 1.f);
    }
}

} // namespace Utils
