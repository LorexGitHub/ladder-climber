#ifndef DK_COIN_H
#define DK_COIN_H

#include <SFML/Graphics.hpp>

class Coin {
public:
    Coin(float x, float y);
    void update(float dt);
    void draw(sf::RenderWindow& win) const;
    sf::FloatRect get_bounds() const;
    bool is_alive() const { return alive; }
    void collect() { alive = false; }
    sf::Vector2f get_pos() const { return pos; }
private:
    sf::Vector2f pos;
    bool alive = true;
    float bob_offset = 0;
    static constexpr float RADIUS = 6.f;
};

#endif
