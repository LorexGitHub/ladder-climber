#include "DonkeyKong.hpp"

DonkeyKong::DonkeyKong(float x, float y) {
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
        win.draw(spr);
    } else {
        win.draw(shape);
    }
}

sf::FloatRect DonkeyKong::get_bounds() const {
    return shape.getGlobalBounds();
}
