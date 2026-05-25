#pragma once
#include <SFML/Graphics.hpp>
#include "../Constants/Constants.h"

class Target {
public:
    Target(sf::Vector2f pos, float depth, GameMode mode);

    void update(float dt);
    void draw(sf::RenderWindow& window) const;
    bool contains(sf::Vector2f point) const;
    bool isAlive() const { return alive; }
    bool isDead()  const { return !alive; }

    bool hit(float dmg = 34.f);   // true = убит

    float        getRadius()   const { return radius; }
    sf::Vector2f getPosition() const { return pos; }
    float        getDepth()    const { return depth; }

private:
    sf::Vector2f pos;
    sf::Vector2f velocity;
    float depth;
    float radius;
    float hp;
    bool  alive;
    GameMode mode;

    float maxRadius;
    float shrinkSpeed;
    float hitFlash;

    sf::Color lerpColor(sf::Color a, sf::Color b, float t) const;
    sf::Color targetColor() const;
};
