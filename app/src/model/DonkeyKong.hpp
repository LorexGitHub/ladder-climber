#ifndef DONKEYKONG_H
#define DONKEYKONG_H

#include <SFML/Graphics.hpp>

/// The Donkey Kong boss character at the top of the level.
/// Has a unique sprite per stage (1-8).
class DonkeyKong {
public:
    DonkeyKong(float x, float y, int stage);

    void draw(sf::RenderWindow& win) const;

    sf::FloatRect get_bounds() const;
    const sf::RectangleShape& get_shape() const { return shape; }

private:
    void load_texture();

    sf::RectangleShape shape{sf::Vector2f{48, 56}};
    sf::Texture texture;
    int monster_stage = 9;
    bool texture_loaded = false;
};

#endif
