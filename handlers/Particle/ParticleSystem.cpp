#include "ParticleSystem.h"
#include <cstdlib>
#include <cmath>
#include <cstdint>
#include <algorithm>

static float rF(float lo, float hi) {
    return lo + (hi - lo) * (std::rand() / (float)RAND_MAX);
}

void ParticleSystem::emit(sf::Vector2f pos, sf::Color color, int count) {
    for (int i = 0; i < count; ++i) {
        float angle = rF(0.f, 2.f * 3.14159f);
        float speed = rF(60.f, 280.f);

        Particle p;
        p.pos     = pos;
        p.vel     = {speed * std::cos(angle), speed * std::sin(angle)};
        p.maxLife = rF(0.3f, 0.8f);
        p.life    = p.maxLife;
        p.color   = color;
        p.size    = rF(2.f, 6.f);
        particles.push_back(p);
    }
}

void ParticleSystem::update(float dt) {
    for (auto& p : particles) {
        p.life -= dt;
        p.pos  += p.vel * dt;
        p.vel  *= (1.f - 3.f * dt);
    }
    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
            [](const Particle& p){ return p.life <= 0.f; }),
        particles.end()
    );
}

void ParticleSystem::draw(sf::RenderWindow& window) const {
    for (const auto& p : particles) {
        float t  = p.life / p.maxLife;
        float r  = p.size * t;

        sf::CircleShape c(r);
        c.setOrigin({r, r});
        c.setPosition(p.pos);

        sf::Color col = p.color;
        col.a = static_cast<uint8_t>(255.f * t);
        c.setFillColor(col);
        window.draw(c);
    }
}
