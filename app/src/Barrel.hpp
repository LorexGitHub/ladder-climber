#ifndef BARREL_H
#define BARREL_H

#include <SFML/Graphics.hpp>

class Barrel {
public:
    Barrel(float x, float y, float vx, int level = 1);

    void update(float dt);
    void draw(sf::RenderWindow& win) const;

    sf::FloatRect get_bounds() const;
    sf::Vector2f get_pos() const { return pos; }
    bool is_alive() const { return alive; }
    void kill() { alive = false; }
    void land_on_surface(float surface_y) {
        pos.y = surface_y;
        vel.y = 0;
        shape.setPosition(pos);
    }
    void set_position(float x, float y) {
        pos = {x, y};
        vel.y = 0;
        shape.setPosition(pos);
    }
    void set_velocity(float vx, float vy) { vel = {vx, vy}; }
    void set_level(int l) { level = l; }
    int get_level() const { return level; }

private:
    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::CircleShape shape{12};
    sf::Texture texture;
    float rotation = 0;
    int level = 1;
    bool alive = true;
};

#endif
