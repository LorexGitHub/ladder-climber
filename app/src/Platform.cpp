#include "Platform.hpp"

Platform::Platform(float x, float y, float w) : shape(sf::Vector2f{w, 14}) {
    shape.setPosition({x, y});
    shape.setFillColor(sf::Color{139, 69, 19}); // brown
}

sf::FloatRect Platform::get_bounds() const {
    return shape.getGlobalBounds();
}