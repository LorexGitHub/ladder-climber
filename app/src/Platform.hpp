#ifndef PLATFORM_H
#define PLATFORM_H

#include <SFML/Graphics.hpp>

struct Ladder {
    sf::FloatRect rect;
    Ladder(float x, float y1, float y2) : rect({x, y1}, {20, y2 - y1}) {}
    sf::FloatRect get_bounds() const { return rect; }
};

class Platform {
public:
    Platform(float x, float y, float w);

    sf::FloatRect get_bounds() const;
    const sf::RectangleShape& get_shape() const { return shape; }

private:
    sf::RectangleShape shape;
};

#endif