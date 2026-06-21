#include "Game.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <fstream>
#include <cmath>

Game::Game()
    : window(sf::VideoMode({800u, 750u}), "Donkey Kong") {

    window.setFramerateLimit(60);

    bool font_ok = font.openFromFile("assets/fonts/arial.ttf");
    if (!font_ok) font_ok = font.openFromFile("assets/fonts/DejaVuSans.ttf");
    if (!font_ok) font_ok = font.openFromFile("/usr/share/fonts/TTF/DejaVuSans.ttf");
    if (!font_ok) font_ok = font.openFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
    if (!font_ok) font_ok = font.openFromFile("/usr/share/fonts/Adwaita/AdwaitaSans-Regular.ttf");

    // ---- Title ----
    title_text.setString("DONKEY KONG");
    title_text.setCharacterSize(48);
    title_text.setFillColor(sf::Color::Yellow);
    auto tb = title_text.getLocalBounds();
    title_text.setOrigin({tb.position.x + tb.size.x / 2, tb.position.y + tb.size.y / 2});
    title_text.setPosition({400, 120});

    // ---- HUD text ----
    status_text.setCharacterSize(40);
    status_text.setFillColor(sf::Color::Yellow);

    // ---- Menu "PLAY" button ----
    menu_btn.setSize({200, 60});
    menu_btn.setFillColor(sf::Color{50, 150, 50});
    menu_btn.setOrigin({100, 30});
    menu_btn.setPosition({400, 340});
    menu_btn_text.setString("PLAY");
    menu_btn_text.setCharacterSize(30);
    menu_btn_text.setFillColor(sf::Color::White);
    auto mt = menu_btn_text.getLocalBounds();
    menu_btn_text.setOrigin({mt.position.x + mt.size.x / 2, mt.position.y + mt.size.y / 2});
    menu_btn_text.setPosition({400, 340});

    // ---- Pause buttons ----
    auto setup_btn = [](sf::RectangleShape& s, sf::Text& t, float x, float y, const std::string& label) {
        s.setSize({160, 50});
        s.setFillColor(sf::Color{60, 60, 180});
        s.setOrigin({80, 25});
        s.setPosition({x, y});
        t.setString(label);
        t.setCharacterSize(24);
        t.setFillColor(sf::Color::White);
        auto lb = t.getLocalBounds();
        t.setOrigin({lb.position.x + lb.size.x / 2, lb.position.y + lb.size.y / 2});
        t.setPosition({x, y});
    };
    setup_btn(pause_resume_btn, pause_resume_text, 280, 280, "RESUME");
    setup_btn(pause_reset_btn, pause_reset_text, 520, 280, "RESET");

    // ---- Princess (top-left) ----
    princess.setSize({20, 36});
    princess.setFillColor(sf::Color::Magenta);
    princess.setPosition({70, 104});
    if (princess_tex.loadFromFile("assets/sprites/princess.png")) {
    }

    // ---- Crown counter ----
    crowns_text.setCharacterSize(24);
    crowns_text.setFillColor(sf::Color::Yellow);
    [[maybe_unused]] bool crown_loaded = crown_tex.loadFromFile("assets/sprites/crown.png");
    load_crowns();

    // ---- Background & tile textures ----
    [[maybe_unused]] bool bg_ok = bg_tex.loadFromFile("assets/sprites/background.png");
    [[maybe_unused]] bool pt_ok = plat_tex.loadFromFile("assets/sprites/platform.png");
    [[maybe_unused]] bool lt_ok = ladder_tex.loadFromFile("assets/sprites/ladder.png");
    [[maybe_unused]] bool lv_ok = lava_tex.loadFromFile("assets/sprites/lava.png");
    bg_shape.setFillColor(sf::Color{20, 20, 40});

    // ---- Platforms (100px gaps, 6 levels) ----
    platforms.emplace_back(50.f, 140.f, 700.f);  // P0 – top (full width)
    platforms.emplace_back(50.f, 254.f, 700.f);  // P1
    platforms.emplace_back(50.f, 368.f, 700.f);  // P2
    platforms.emplace_back(50.f, 482.f, 700.f);  // P3
    platforms.emplace_back(50.f, 596.f, 700.f);  // P4
    platforms.emplace_back(50.f, 710.f, 700.f);  // P5 – ground

    // ---- Ladders (alternating sides, bottom → top) ----
    ladders.emplace_back(680.f, 596.f, 710.f);  // P5→P4  (right)
    ladders.emplace_back(130.f, 482.f, 596.f);  // P4→P3  (left)
    ladders.emplace_back(680.f, 368.f, 482.f);  // P3→P2  (right)
    ladders.emplace_back(130.f, 254.f, 368.f);  // P2→P1  (left)
    ladders.emplace_back(680.f, 140.f, 254.f);  // P1→P0  (right)

    // ---- Player start (left side of ground) ----
    player.set_position(100.f, 710.f);
}

void Game::start_game() {
    state = State::Playing;
    paused = false;
    barrels.clear();
    barrel_timer = 0;
    player.set_position(100.f, 710.f);
    player.set_dead(false);
    dk = DonkeyKong(60.f, 140.f);
    player.set_climbing(false);
    // Start with barrels on P0-P4 (P5 is player spawn)
    for (int lvl = 0; lvl < 5; lvl++) {
        float y = 140.f + lvl * 114.f;
        if (lvl % 2 == 0)
            barrels.push_back(std::make_unique<Barrel>(60.f, y, 180.f, lvl));
        else
            barrels.push_back(std::make_unique<Barrel>(728.f, y, -180.f, lvl));
    }
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Time dt = clock.restart();
        handle_input(dt.asSeconds());
        if (state == State::Playing && !paused)
            update(dt.asSeconds());
        draw();
    }
}

void Game::handle_input(float dt) {
    pause_cd -= dt;
    while (auto ev = window.pollEvent()) {
        if (ev->is<sf::Event::Closed>())
            window.close();

        if (auto* mb = ev->getIf<sf::Event::MouseButtonPressed>()) {
            sf::Vector2f wp = window.mapPixelToCoords(mb->position);

            if (state == State::Playing && paused) {
                if (pause_resume_btn.getGlobalBounds().contains(wp))
                    paused = false;
                else if (pause_reset_btn.getGlobalBounds().contains(wp))
                    start_game();
            }

            if (state == State::Menu || state == State::GameOver || state == State::Won) {
                if (menu_btn.getGlobalBounds().contains(wp))
                    start_game();
            }
        }
    }

    if (state == State::Playing) {
        if (pause_cd <= 0 &&
            (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape) ||
             sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P))) {
            paused = !paused;
            pause_cd = 1.f;
        }

        bool on_ladder = false;
        for (auto& l : ladders) {
            if (player.get_bounds().findIntersection(l.get_bounds()).has_value()) {
                on_ladder = true;
                break;
            }
        }

        if (on_ladder) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
                player.climb(-180.f);
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
                player.climb(180.f);
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
                player.move_left();
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
                player.move_right();
            else
                player.stop_on_ladder();
        } else {
            if (player.is_climbing())
                player.set_climbing(false);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
                player.move_left();
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
                player.move_right();
            else
                player.stop_horizontal();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
            player.jump();
    }
}

void Game::update(float dt) {
    // Lava animation
    lava_anim += dt * 30.f;
    if (lava_anim > 800) lava_anim = 0;

    // ----- Player -----
    player.update(dt);

    // Clamp player to ladder bounds; auto-stop at endpoints
    if (player.is_climbing()) {
        for (auto& l : ladders) {
            if (player.get_bounds().findIntersection(l.get_bounds()).has_value()) {
                float ltop = l.get_bounds().position.y;
                float lbot = ltop + l.get_bounds().size.y;
                float py = player.get_pos().y;
                if (py < ltop) {
                    player.set_position(player.get_pos().x, ltop);
                    player.set_climbing(false);
                }
                if (py > lbot) {
                    player.set_position(player.get_pos().x, lbot);
                    player.set_climbing(false);
                }
                break;
            }
        }
    }

    // Player-platform collision (skip while climbing)
    if (!player.is_climbing()) {
        bool on_any = false;
        for (auto& p : platforms) {
            auto overlap = player.get_bounds().findIntersection(p.get_bounds());
            if (overlap.has_value()) {
                float player_bottom = player.get_pos().y + 16;
                float platform_top  = p.get_bounds().position.y;
                if (player_bottom >= platform_top && player_bottom < platform_top + 24) {
                    float p_left  = p.get_bounds().position.x + 12;
                    float p_right = p.get_bounds().position.x + p.get_bounds().size.x - 12;
                    float px = player.get_pos().x;
                    if (px < p_left)  px = p_left;
                    if (px > p_right) px = p_right;
                    player.set_position(px, platform_top);
                    on_any = true;
                }
            }
        }
        if (!on_any)
            player.set_on_ground(false);
    }

    // ----- Barrels -----
    barrel_timer += dt;
    if (barrel_timer >= BARREL_INTERVAL) {
        spawn_barrel();
        barrel_timer = 0;
    }

    for (auto it = barrels.begin(); it != barrels.end();) {
        (*it)->update(dt);

        // Barrel-platform collision
        for (auto& p : platforms) {
            auto ov = (*it)->get_bounds().findIntersection(p.get_bounds());
            if (ov.has_value()) {
                float ptop = p.get_bounds().position.y;
                float bbot = (*it)->get_pos().y + 12;
                if (bbot >= ptop - 4 && bbot < ptop + 20) {
                    (*it)->land_on_surface(ptop - 12);
                    break;
                }
            }
        }

        // Barrel cascade: even levels roll right, odd roll left
        int lvl = (*it)->get_level();
        float bx = (*it)->get_pos().x;
        bool at_edge = (lvl % 2 == 0) ? (bx > 740) : (bx < 60);
        if (at_edge && lvl < 5) {
            int next = lvl + 1;
            float next_y = 140.f + next * 114.f;
            float new_x, new_vx;
            if (next % 2 == 0) {
                new_x = 60.f;
                new_vx = 180.f;
            } else {
                new_x = 728.f;
                new_vx = -180.f;
            }
            (*it)->set_position(new_x, next_y);
            (*it)->set_velocity(new_vx, 0);
            (*it)->set_level(next);
        }

        if (!(*it)->is_alive())
            it = barrels.erase(it);
        else
            ++it;
    }

    // ----- Collisions -----
    check_collisions();

    // Lava kills
    if (player.get_pos().y > 710 && state == State::Playing) {
        state = State::GameOver;
        player.set_dead(true);
    }

    // ----- Win? (player must reach princess at top-left) -----
    if (player.get_pos().y < 140 && player.get_pos().x < 100 && state == State::Playing) {
        state = State::Won;
        crowns++;
        save_crowns();
    }
}

void Game::spawn_barrel() {
    // Spawn on P0 (top), left side, rolling right
    barrels.push_back(std::make_unique<Barrel>(60.f, 140.f, 180.f, 0));
}

void Game::check_collisions() {
    for (auto& b : barrels) {
        if (player.get_bounds().findIntersection(b->get_bounds()).has_value()) {
            state = State::GameOver;
            player.set_dead(true);
            return;
        }
    }
}

void Game::load_crowns() {
    std::ifstream f("crowns.dat");
    if (f) { f >> crowns; }
}

void Game::save_crowns() {
    std::ofstream f("crowns.dat");
    f << crowns;
}

void Game::draw() {
    window.clear(sf::Color{20, 20, 40});

    if (state == State::Menu) {
        window.draw(title_text);
        window.draw(menu_btn);
        window.draw(menu_btn_text);
        // Crown counter
        std::string cstr = std::to_string(crowns);
        crowns_text.setString(cstr);
        crowns_text.setPosition({400, 200});
        crowns_text.setOrigin({crowns_text.getLocalBounds().size.x / 2, 0});
        window.draw(crowns_text);
        if (crown_tex.getSize().x > 0) {
            sf::Sprite crown_spr(crown_tex);
            crown_spr.setOrigin({16, 16});
            crown_spr.setPosition({400 - crowns_text.getLocalBounds().size.x / 2 - 24, 204});
            window.draw(crown_spr);
        }
        window.display();
        return;
    }

    // ---- Game elements (Playing / GameOver / Won) ----
    // Background
    if (bg_tex.getSize().x > 0) {
        sf::Sprite bg_spr(bg_tex);
        window.draw(bg_spr);
    } else {
        window.draw(bg_shape);
    }

    // Lava at bottom
    if (lava_tex.getSize().x > 0) {
        sf::Sprite lava_spr(lava_tex);
        lava_spr.setPosition({0.f, 710.f});
        window.draw(lava_spr);
        // Animated glow
        sf::RectangleShape glow({800, 40});
        glow.setPosition({0, 710});
        int a = 20 + int(15 * std::sin(lava_anim * 0.1));
        glow.setFillColor(sf::Color(std::uint8_t(255), std::uint8_t(200), std::uint8_t(50), std::uint8_t(a)));
        window.draw(glow);
    } else {
        sf::RectangleShape lava_fill({800, 40});
        lava_fill.setPosition({0, 710});
        lava_fill.setFillColor(sf::Color{200, 50, 10});
        window.draw(lava_fill);
    }

    // Platforms
    if (plat_tex.getSize().x > 0) {
        for (auto& p : platforms) {
            sf::Sprite p_spr(plat_tex);
            p_spr.setPosition(p.get_bounds().position);
            window.draw(p_spr);
        }
    } else {
        for (auto& p : platforms)
            window.draw(p.get_shape());
    }

    // Ladders
    if (ladder_tex.getSize().x > 0) {
        for (auto& l : ladders) {
            sf::Sprite l_spr(ladder_tex);
            l_spr.setPosition({l.get_bounds().position.x, l.get_bounds().position.y});
            l_spr.setScale({1, l.get_bounds().size.y / 100.f});
            window.draw(l_spr);
        }
    } else {
        sf::RectangleShape ladder_shape({16, 120});
        ladder_shape.setFillColor(sf::Color{180, 180, 180});
        for (auto& l : ladders) {
            ladder_shape.setPosition({l.get_bounds().position.x, l.get_bounds().position.y});
            ladder_shape.setSize({16, l.get_bounds().size.y});
            window.draw(ladder_shape);
        }
    }

    dk.draw(window);
    if (princess_tex.getSize().x > 0) {
        sf::Sprite spr(princess_tex);
        spr.setPosition({70, 104});
        window.draw(spr);
    } else
        window.draw(princess);
    player.draw(window);

    for (auto& b : barrels)
        b->draw(window);

    // Crown counter HUD
    std::string cstr = std::to_string(crowns);
    crowns_text.setString(cstr);
    crowns_text.setPosition({770, 10});
    crowns_text.setOrigin({crowns_text.getLocalBounds().size.x, 0});
    window.draw(crowns_text);
    if (crown_tex.getSize().x > 0) {
        sf::Sprite crown_spr(crown_tex);
        crown_spr.setOrigin({16, 16});
        crown_spr.setPosition({770 - crowns_text.getLocalBounds().size.x - 8, 18});
        window.draw(crown_spr);
    }

    if (paused) {
        sf::RectangleShape overlay({800, 750});
        overlay.setFillColor(sf::Color{0, 0, 0, 150});
        window.draw(overlay);

        status_text.setString("PAUSED");
        status_text.setPosition({320, 180});
        window.draw(status_text);

        window.draw(pause_resume_btn);
        window.draw(pause_resume_text);
        window.draw(pause_reset_btn);
        window.draw(pause_reset_text);
    }

    if (state == State::GameOver) {
        status_text.setString("GAME OVER");
        status_text.setPosition({280, 200});
        window.draw(status_text);
        menu_btn.setPosition({400, 340});
        menu_btn_text.setString("PLAY AGAIN");
        auto mt = menu_btn_text.getLocalBounds();
        menu_btn_text.setOrigin({mt.position.x + mt.size.x / 2, mt.position.y + mt.size.y / 2});
        menu_btn_text.setPosition({400, 340});
        window.draw(menu_btn);
        window.draw(menu_btn_text);
    }

    if (state == State::Won) {
        status_text.setString("YOU WIN!");
        status_text.setPosition({300, 200});
        window.draw(status_text);
        std::string cstr = "CROWNS: " + std::to_string(crowns);
        crowns_text.setString(cstr);
        crowns_text.setPosition({400, 260});
        crowns_text.setOrigin({crowns_text.getLocalBounds().size.x / 2, 0});
        window.draw(crowns_text);
        menu_btn.setPosition({400, 340});
        menu_btn_text.setString("PLAY AGAIN");
        auto mt = menu_btn_text.getLocalBounds();
        menu_btn_text.setOrigin({mt.position.x + mt.size.x / 2, mt.position.y + mt.size.y / 2});
        menu_btn_text.setPosition({400, 340});
        window.draw(menu_btn);
        window.draw(menu_btn_text);
    }

    window.display();
}