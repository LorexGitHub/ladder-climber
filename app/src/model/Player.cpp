#include "Player.hpp"
#include <algorithm>

Player::Player() {
    shape.setFillColor(sf::Color::Green);
    shape.setOrigin({12, 32});
    [[maybe_unused]] bool loaded = texture.loadFromFile("assets/sprites/player.png");
}

void Player::move_left()  { dir = -1; climbing = false; }
void Player::move_right() { dir = 1;  climbing = false; }
void Player::stop_horizontal() { dir = 0; }

float Player::get_current_speed() const {
    return SPEED * (1.f + bunny_count * BUNNY_BOOST);
}

void Player::jump() {
    if (climbing || dead) return;
    if (jumps_left <= 0) return;

    jumps_left--;

    if (!on_ground)
        bunny_count++;
    else
        bunny_count = 0;

    vel.y = JUMP_VEL;
    on_ground = false;
}

void Player::climb(float speed) {
    climbing = true;
    vel.y = speed;
    vel.x = 0;
    dir = 0;
}

void Player::stop_on_ladder() {
    climbing = true;
    vel.y = 0;
    vel.x = 0;
    dir = 0;
}

void Player::bump_head(float y) {
    pos.y = y;
    vel.y = 0;
    shape.setPosition(pos);
}

void Player::set_on_ground(bool g) {
    on_ground = g;
    if (g) {
        jumps_left = MAX_JUMPS;
        bunny_count = 0;
    }
}

void Player::update(float dt) {
    if (dead) return;

    if (climbing) {
        pos.y += vel.y * dt;
        shape.setPosition(pos);
        if (vel.y != 0) {
            anim_timer += dt;
            if (anim_timer > 0.2f) {
                anim_frame = 1 - anim_frame;
                anim_timer = 0;
            }
        } else {
            anim_frame = 0;
        }
        return;
    }

    float speed = get_current_speed();

    if (on_ground) {
        vel.x = dir * speed;
    } else if (dir != 0) {
        float target = dir * speed;
        vel.x += (target - vel.x) * std::min(1.0f, AIR_ACCEL * dt);
    }
    vel.y += GRAVITY * dt;
    pos += vel * dt;

    if (pos.y > 750) {
        pos.y = 750;
        vel.y = 0;
        set_on_ground(true);
    }
    if (pos.x < 12)  pos.x = 12;
    if (pos.x > 788) pos.x = 788;

    shape.setPosition(pos);

    if (on_ground && dir != 0) {
        anim_timer += dt;
        if (anim_timer > 0.15f) {
            anim_frame = 1 - anim_frame;
            anim_timer = 0;
        }
    } else {
        anim_frame = 0;
    }
}

void Player::set_position(float x, float y) {
    pos = {x, y};
    vel = {0, 0};
    set_on_ground(true);
    shape.setPosition(pos);
}

sf::FloatRect Player::get_bounds() const {
    return shape.getGlobalBounds();
}

void Player::draw(sf::RenderWindow& w) const {
    if (texture.getSize().x > 0) {
        sf::Sprite spr(texture);
        spr.setOrigin({12, 32});
        spr.setPosition(pos);
        int frame = dead ? 2 : anim_frame;
        spr.setTextureRect({{frame * 24, 0}, {24, 32}});
        if (dir < 0 && !dead)
            spr.setScale({-1.f, 1.f});
        w.draw(spr);
    } else {
        w.draw(shape);
    }
}
