#include "Coin.hpp"
#include <cmath>
#include <cstdlib>

Coin::Coin(float x, float y) : pos(x, y) {
    bob_offset = static_cast<float>(std::rand() % 100);
}

void Coin::update(float dt) {
    bob_offset += dt * 3.f;
}

void Coin::draw(sf::RenderWindow& win) const {
    if (!alive) return;
    float bob_y = std::sin(bob_offset) * 3.f;
    sf::CircleShape c(RADIUS);
    c.setFillColor(sf::Color::Yellow);
    c.setOrigin({RADIUS, RADIUS});
    c.setPosition({pos.x + 12.f, pos.y + bob_y - 7.f});
    win.draw(c);
    sf::CircleShape inner(RADIUS * 0.5f);
    inner.setFillColor(sf::Color(255, 215, 0));
    inner.setOrigin({RADIUS * 0.5f, RADIUS * 0.5f});
    inner.setPosition({pos.x + 12.f, pos.y + bob_y - 7.f});
    win.draw(inner);
}

sf::FloatRect Coin::get_bounds() const {
    return {{pos.x + 12.f - RADIUS, pos.y - 7.f - RADIUS}, {RADIUS * 2, RADIUS * 2}};
}
