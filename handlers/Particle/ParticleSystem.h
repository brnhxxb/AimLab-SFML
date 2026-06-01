#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

//отвечает за визуал
struct Particle {
    sf::Vector2f pos;
    sf::Vector2f vel;
    float life;
    float maxLife;
    sf::Color color;
    float size;
};

class ParticleSystem {
public:
    void emit(sf::Vector2f pos, sf::Color color, int count = 14);
    void update(float dt);
    void draw(sf::RenderWindow& window) const;
    void clear() { particles.clear(); }

private:
    std::vector<Particle> particles;
};
