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
    void bump_head(float y);
    void set_on_ground(bool g);
    void set_dead(bool d) { dead = d; }
    void draw(sf::RenderWindow& w) const;

    sf::FloatRect get_bounds() const;
    sf::Vector2f get_pos() const { return pos; }
    sf::Vector2f get_vel() const { return vel; }
    bool is_on_ground() const { return on_ground; }
    bool is_climbing() const { return climbing; }
    void set_climbing(bool c) { climbing = c; }
    bool is_dead() const { return dead; }
    int get_jumps_left() const { return jumps_left; }
    int get_bunny_count() const { return bunny_count; }

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
    int dir = 0;
    int jumps_left = 2;
    int bunny_count = 0;

    float get_current_speed() const;

    static constexpr float SPEED = 200.f;
    static constexpr float JUMP_VEL = -330.f;
    static constexpr float GRAVITY = 900.f;
    static constexpr float AIR_ACCEL = 15.f;
    static constexpr int MAX_JUMPS = 2;
    static constexpr float BUNNY_BOOST = 0.25f;
};

#endif
