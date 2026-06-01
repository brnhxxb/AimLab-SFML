#include "Target.h"
#include <cmath>
#include <cstdlib>
#include <cstdint>

static float randF(float lo, float hi) {
    return lo + (hi - lo) * (std::rand() / (float)RAND_MAX);
}

Target::Target(sf::Vector2f pos, float depth, GameMode mode)
    : pos(pos), depth(depth), mode(mode), hp(100.f), alive(true), hitFlash(0.f)
{
    radius    = TARGET_RADIUS_MAX - depth * (TARGET_RADIUS_MAX - TARGET_RADIUS_MIN);
    maxRadius = radius;

    if (mode == GameMode::MOVING) {
        float speed = randF(60.f, 180.f);
        float angle = randF(0.f, 2.f * 3.14159f);
        velocity = {speed * std::cos(angle), speed * std::sin(angle)};
    } else {
        velocity = {0.f, 0.f};
    }

    shrinkSpeed = (mode == GameMode::SHRINKING) ? randF(12.f, 30.f) : 0.f;
}

bool Target::hit(float dmg) {
    if (!alive) return false;
    hp -= dmg;
    hitFlash = 0.15f;
    if (hp <= 0.f) { hp = 0.f; alive = false; return true; }
    return false;
}

void Target::update(float dt) {
    if (!alive) return;

    hitFlash -= dt;
    if (hitFlash < 0.f) hitFlash = 0.f;

    if (mode == GameMode::MOVING) {
        pos += velocity * dt;
        if (pos.x - radius < 0.f)                  { pos.x = radius;                   velocity.x =  std::abs(velocity.x); }
        if (pos.x + radius > WINDOW_W)             { pos.x = WINDOW_W - radius;         velocity.x = -std::abs(velocity.x); }
        if (pos.y - radius < 80.f)                 { pos.y = 80.f + radius;             velocity.y =  std::abs(velocity.y); }
        if (pos.y + radius > WINDOW_H - 40.f)       { pos.y = WINDOW_H - 40.f - radius;  velocity.y = -std::abs(velocity.y); }
    }

    if (mode == GameMode::SHRINKING) {
        radius -= shrinkSpeed * dt;
        if (radius <= 0.f) { radius = 0.f; alive = false; }
    }
}

bool Target::contains(sf::Vector2f point) const {
    if (!alive) return false;
    float dx = point.x - pos.x;
    float dy = point.y - pos.y;
    return (dx * dx + dy * dy) <= (radius * radius);
}

sf::Color Target::lerpColor(sf::Color a, sf::Color b, float t) const {
    return sf::Color(
        static_cast<uint8_t>(a.r + (b.r - a.r) * t),
        static_cast<uint8_t>(a.g + (b.g - a.g) * t),
        static_cast<uint8_t>(a.b + (b.b - a.b) * t),
        255
    );
}

sf::Color Target::targetColor() const {
    sf::Color base = lerpColor(COL_TARGET_NEAR, COL_TARGET_FAR, depth);
    if (hitFlash > 0.f) {
        float t = hitFlash / 0.15f;
        base = lerpColor(base, sf::Color(255, 255, 255), t * 0.7f);
    }
    return base;
}

void Target::draw(sf::RenderWindow& window) const {
    if (!alive || radius <= 1.f) return;

    // Тень
    sf::CircleShape shadow(radius + 4.f);
    shadow.setOrigin({radius + 4.f, radius + 4.f});
    shadow.setPosition({pos.x + 4.f * (1.f - depth), pos.y + 4.f});
    shadow.setFillColor(sf::Color(0, 0, 0, static_cast<uint8_t>(80 * (1.f - depth))));
    window.draw(shadow);

    // Кольцо
    sf::CircleShape ring(radius);
    ring.setOrigin({radius, radius});
    ring.setPosition(pos);
    ring.setFillColor(sf::Color::Transparent);
    ring.setOutlineThickness(2.f);
    sf::Color ringCol = targetColor();
    ringCol.a = 200;
    ring.setOutlineColor(ringCol);
    window.draw(ring);

    // Тело
    sf::CircleShape body(radius - 2.f);
    body.setOrigin({radius - 2.f, radius - 2.f});
    body.setPosition(pos);
    sf::Color bodyCol = targetColor();
    bodyCol.a = static_cast<uint8_t>(200 - static_cast<int>(80 * depth));
    body.setFillColor(bodyCol);
    window.draw(body);

    // Внутренний круг
    float innerR = radius * 0.35f;
    sf::CircleShape inner(innerR);
    inner.setOrigin({innerR, innerR});
    inner.setPosition(pos);
    inner.setFillColor(sf::Color(255, 230, 200,
        static_cast<uint8_t>(220 - static_cast<int>(60 * depth))));
    window.draw(inner);

    // HP-бар
    float barW = radius * 2.2f;
    float barX = pos.x - barW / 2.f;
    float barY = pos.y + radius + 6.f;

    sf::RectangleShape barBg({barW, TARGET_HP_BAR_H});
    barBg.setPosition({barX, barY});
    barBg.setFillColor(sf::Color(120, 90, 100, 200));
    barBg.setOutlineColor(sf::Color(60, 60, 80));
    barBg.setOutlineThickness(1.f);
    window.draw(barBg);

    float    hpFrac = hp / 100.f;
    sf::Color hpCol = lerpColor(COL_HP_LOW, COL_HP_FULL, hpFrac);
    sf::RectangleShape barFill({barW * hpFrac, TARGET_HP_BAR_H});
    barFill.setPosition({barX, barY});
    barFill.setFillColor(hpCol);
    window.draw(barFill);
}
