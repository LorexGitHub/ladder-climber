#include "DonkeyKong.hpp"

DonkeyKong::DonkeyKong(float x, float y, int stage) : monster_stage(stage) {
    shape.setFillColor(sf::Color{80, 60, 30});
    shape.setOrigin({24, 56});
    shape.setPosition({x, y});
    [[maybe_unused]] bool loaded = texture.loadFromFile("assets/sprites/donkey_kong.png");
}

void DonkeyKong::draw(sf::RenderWindow& win) const {
    if (texture.getSize().x > 0) {
        sf::Sprite spr(texture);
        spr.setOrigin({24, 56});
        spr.setPosition(shape.getPosition());
        if (monster_stage < 9) {
            sf::Color tints[] = {
                {220, 120, 120}, {120, 220, 120}, {120, 120, 220},
                {220, 220, 100}, {200, 120, 220}, {100, 220, 220},
                {220, 180, 100}, {180, 120, 220}
            };
            spr.setColor(tints[(monster_stage - 1) % 8]);
        }
        win.draw(spr);
    } else {
        win.draw(shape);
    }
}

sf::FloatRect DonkeyKong::get_bounds() const {
    return shape.getGlobalBounds();
}
