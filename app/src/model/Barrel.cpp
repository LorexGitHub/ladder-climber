#include "Barrel.hpp"
#include <cmath>

Barrel::Barrel(float x, float y, float vx, int lvl) : pos{x, y}, vel{vx, 0}, level(lvl) {
    shape.setOrigin({12, 12});
    shape.setPosition(pos);
    shape.setFillColor(sf::Color{200, 50, 50});
    [[maybe_unused]] bool loaded = texture.loadFromFile("assets/sprites/barrel.png");
}

void Barrel::update(float dt) {
    vel.y += 600.f * dt;
    pos += vel * dt;
    shape.setPosition(pos);
    rotation += vel.x * 3.f * dt;

    if (pos.y > 750 || pos.x < -50 || pos.x > 850)
        alive = false;
}

sf::FloatRect Barrel::get_bounds() const {
    return shape.getGlobalBounds();
}

void Barrel::draw(sf::RenderWindow& win) const {
    if (texture.getSize().x > 0) {
        sf::Sprite spr(texture);
        spr.setOrigin({12, 12});
        spr.setPosition(pos);
        spr.setRotation(sf::degrees(rotation));
        win.draw(spr);
    } else {
        win.draw(shape);
    }
}
