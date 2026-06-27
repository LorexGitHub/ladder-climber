#include "PowerUp.hpp"
#include <cmath>

PowerUp::PowerUp(float x, float y) : pos(x, y) {}

void PowerUp::update(float dt) {
    if (!active) return;
    anim_timer += dt * 4.f;
}

void PowerUp::draw(sf::RenderWindow& win) const {
    if (!active) return;
    float pulse = 1.f + std::sin(anim_timer) * 0.2f;
    sf::CircleShape glow(SIZE * pulse * 2.f);
    glow.setFillColor(sf::Color(100, 255, 255, 50));
    glow.setOrigin({SIZE * pulse * 2.f, SIZE * pulse * 2.f});
    glow.setPosition({pos.x + 12.f, pos.y - 7.f});
    win.draw(glow);
    sf::CircleShape star(SIZE * pulse, 4);
    star.setFillColor(sf::Color::Cyan);
    star.setOrigin({SIZE * pulse, SIZE * pulse});
    star.setPosition({pos.x + 12.f, pos.y - 7.f});
    star.setRotation(sf::degrees(anim_timer * 30.f));
    win.draw(star);
}

sf::FloatRect PowerUp::get_bounds() const {
    float s = SIZE * 1.5f;
    return {{pos.x + 12.f - s, pos.y - 7.f - s}, {s * 2, s * 2}};
}
