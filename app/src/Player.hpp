#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

class Player {
public:
    Player();

    void move_left();
    void move_right();
    void jump();
    void climb(float speed);
    void stop_on_ladder();
    void stop_horizontal();
    void update(float dt);

    void set_position(float x, float y);
    void set_on_ground(bool g) { on_ground = g; }
    void set_dead(bool d) { dead = d; }
    void draw(sf::RenderWindow& w) const;

    sf::FloatRect get_bounds() const;
    sf::Vector2f get_pos() const { return pos; }
    bool is_on_ground() const { return on_ground; }
    bool is_climbing() const { return climbing; }
    void set_climbing(bool c) { climbing = c; }
    bool is_dead() const { return dead; }

    const sf::RectangleShape& get_shape() const { return shape; }

private:
    sf::Vector2f pos;
    sf::Vector2f vel{0, 0};
    sf::RectangleShape shape{sf::Vector2f{24, 32}};
    sf::Texture texture;
    float anim_timer = 0;
    int anim_frame = 0;
    bool on_ground = false;
    bool climbing = false;
    bool dead = false;
    int dir = 0; // -1 left, 0 none, 1 right

    static constexpr float SPEED = 200.f;
    static constexpr float JUMP_VEL = -330.f;
    static constexpr float GRAVITY = 900.f;
    static constexpr float AIR_ACCEL = 15.f;
};

#endif
