#include "DonkeyKong.hpp"
#include <cstdio>

DonkeyKong::DonkeyKong(float x, float y, int stage) : monster_stage(stage) {
    shape.setFillColor(sf::Color{80, 60, 30});
    shape.setOrigin({24, 56});
    shape.setPosition({x, y});
    load_texture();
}

void DonkeyKong::load_texture() {
    char path[64];
    if (monster_stage < 9)
        std::snprintf(path, sizeof(path), "assets/sprites/boss_%d.png", monster_stage);
    else
        std::snprintf(path, sizeof(path), "assets/sprites/donkey_kong.png");
    texture_loaded = texture.loadFromFile(path);
}

void DonkeyKong::draw(sf::RenderWindow& win) const {
    if (texture_loaded) {
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