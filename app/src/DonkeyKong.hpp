#ifndef DONKEYKONG_H
#define DONKEYKONG_H

#include <SFML/Graphics.hpp>

class DonkeyKong {
public:
    DonkeyKong(float x, float y);

    void draw(sf::RenderWindow& win) const;

    sf::FloatRect get_bounds() const;
    const sf::RectangleShape& get_shape() const { return shape; }

private:
    sf::RectangleShape shape{sf::Vector2f{48, 56}};
    sf::Texture texture;
};

#endif
