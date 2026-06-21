#include "Game.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <cmath>

int Game::get_stage() const {
    return crowns + 1;
}

void Game::setup_stage() {
    platforms.clear();
    ladders.clear();
    platform_segments.clear();

    stage = get_stage();

    std::vector<float> ys;
    if (stage == 1)
        ys = {140.f, 330.f, 520.f, 710.f};
    else
        ys = {140.f, 254.f, 368.f, 482.f, 596.f, 710.f};

    for (float y : ys)
        platforms.emplace_back(50.f, y, 700.f);

    int n = ys.size();
    for (int i = 0; i < n - 1; i++) {
        float x = ((n - 2 - i) % 2 == 0) ? 680.f : 130.f;
        ladders.emplace_back(x, ys[i], ys[i + 1]);
    }

    platform_segments.resize(platforms.size(), std::vector<bool>(10, true));

    if (stage >= 3) {
        int num_with_holes = stage - 2;
        if (num_with_holes > (int)platforms.size())
            num_with_holes = (int)platforms.size();
        for (int h = 0; h < num_with_holes; h++) {
            int pi = (crowns * 7 + h * 13) % (int)platforms.size();
            int seg = ((pi * 100 + crowns * 50 + h * 37) % 8 + 8) % 8 + 1;
            if (!platform_segments[pi][seg]) {
                pi = (pi + 1) % (int)platforms.size();
                seg = ((pi * 100 + crowns * 50) % 8 + 8) % 8 + 1;
            }
            platform_segments[pi][seg] = false;
        }
    }
}

sf::FloatRect Game::get_segment_rect(int pi, int s) const {
    auto& p = platforms[pi];
    auto pos = p.get_bounds().position;
    return {{pos.x + s * 70.f, pos.y}, {70.f, 14.f}};
}

bool Game::is_solid(int pi, int s) const {
    return pi < (int)platform_segments.size() && s < 10 && platform_segments[pi][s];
}

Game::Game()
    : window(sf::VideoMode({800u, 750u}), "Donkey Kong") {

    window.setFramerateLimit(60);

    bool font_ok = font.openFromFile("assets/fonts/arial.ttf");
    if (!font_ok) font_ok = font.openFromFile("assets/fonts/DejaVuSans.ttf");
    if (!font_ok) font_ok = font.openFromFile("/usr/share/fonts/TTF/DejaVuSans.ttf");
    if (!font_ok) font_ok = font.openFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
    if (!font_ok) font_ok = font.openFromFile("/usr/share/fonts/Adwaita/AdwaitaSans-Regular.ttf");

    title_text.setString("DONKEY KONG");
    title_text.setCharacterSize(48);
    title_text.setFillColor(sf::Color::Yellow);
    auto tb = title_text.getLocalBounds();
    title_text.setOrigin({tb.position.x + tb.size.x / 2, tb.position.y + tb.size.y / 2});
    title_text.setPosition({400, 120});

    status_text.setCharacterSize(40);
    status_text.setFillColor(sf::Color::Yellow);

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

    princess.setSize({20, 36});
    princess.setFillColor(sf::Color::Magenta);
    princess.setPosition({70, 104});
    [[maybe_unused]] bool pt_loaded = princess_tex.loadFromFile("assets/sprites/princess.png");

    crowns_text.setCharacterSize(24);
    crowns_text.setFillColor(sf::Color::Yellow);
    [[maybe_unused]] bool crown_loaded = crown_tex.loadFromFile("assets/sprites/crown.png");

    stage_text.setCharacterSize(18);
    stage_text.setFillColor(sf::Color{180, 180, 200});

    [[maybe_unused]] bool bg_ok = bg_tex.loadFromFile("assets/sprites/background.png");
    [[maybe_unused]] bool p_ok = plat_tex.loadFromFile("assets/sprites/platform.png");
    [[maybe_unused]] bool lt_ok = ladder_tex.loadFromFile("assets/sprites/ladder.png");
    [[maybe_unused]] bool lv_ok = lava_tex.loadFromFile("assets/sprites/lava.png");
    bg_shape.setFillColor(sf::Color{20, 20, 40});

    setup_stage();
    player.set_position(100.f, 710.f);
}

void Game::start_game() {
    if (crowns >= 9)
        crowns = 0;
    setup_stage();
    state = State::Playing;
    paused = false;
    barrels.clear();
    barrel_timer = 0;
    player.set_position(100.f, 710.f);
    player.set_dead(false);
    dk = DonkeyKong(60.f, 140.f, stage);
    player.set_climbing(false);

    for (int lvl = 0; lvl < (int)platforms.size() - 1; lvl++) {
        float y = platforms[lvl].get_bounds().position.y;
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

    // Stage skip keys (U = back, I = forward)
    skip_cd -= dt;
    if (skip_cd <= 0) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::U)) {
            if (crowns > 0) crowns--;
            else crowns = 0;
            setup_stage();
            skip_cd = 0.3f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::I)) {
            if (crowns < 8) crowns++;
            else crowns = 8;
            setup_stage();
            skip_cd = 0.3f;
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
    lava_anim += dt * 30.f;
    if (lava_anim > 800) lava_anim = 0;

    player.update(dt);

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

    // Player-platform collision using segments
    if (!player.is_climbing()) {
        bool on_any = false;
        for (int pi = 0; pi < (int)platforms.size(); pi++) {
            auto overlap = player.get_bounds().findIntersection(platforms[pi].get_bounds());
            if (overlap.has_value()) {
                float player_bottom = player.get_pos().y + 16;
                float platform_top = platforms[pi].get_bounds().position.y;
                if (player_bottom >= platform_top && player_bottom < platform_top + 24) {
                    // Check if player is over a solid segment
                    bool on_solid = false;
                    for (int s = 0; s < 10; s++) {
                        if (!is_solid(pi, s)) continue;
                        auto seg = get_segment_rect(pi, s);
                        if (player.get_bounds().findIntersection(seg).has_value()) {
                            on_solid = true;
                            break;
                        }
                    }
                    if (on_solid) {
                        // Clamp to full platform bounds, not segment boundaries
                        auto ppos = platforms[pi].get_bounds();
                        float p_left  = ppos.position.x + 12;
                        float p_right = ppos.position.x + ppos.size.x - 12;
                        float px = player.get_pos().x;
                        if (px < p_left)  px = p_left;
                        if (px > p_right) px = p_right;
                        player.set_position(px, platform_top);
                        on_any = true;
                        break;
                    }
                }
            }
            }
        if (!on_any)
            player.set_on_ground(false);
    }

    // Barrels
    barrel_timer += dt;
    if (barrel_timer >= BARREL_INTERVAL) {
        spawn_barrel();
        barrel_timer = 0;
    }

    for (auto it = barrels.begin(); it != barrels.end();) {
        (*it)->update(dt);

        // Barrel-platform collision with segments
        for (int pi = 0; pi < (int)platforms.size(); pi++) {
            auto ov = (*it)->get_bounds().findIntersection(platforms[pi].get_bounds());
            if (ov.has_value()) {
                float ptop = platforms[pi].get_bounds().position.y;
                float bbot = (*it)->get_pos().y + 12;
                if (bbot >= ptop - 4 && bbot < ptop + 20) {
                    // Check if barrel is over a solid segment
                    for (int s = 0; s < 10; s++) {
                        if (!is_solid(pi, s)) continue;
                        auto seg = get_segment_rect(pi, s);
                        if ((*it)->get_bounds().findIntersection(seg).has_value()) {
                            (*it)->land_on_surface(ptop - 12);
                            (*it)->set_level(pi);
                            (*it)->set_velocity((pi % 2 == 0) ? 180.f : -180.f, 0);
                            goto barrel_landed;
                        }
                    }
                }
            }
        }
        barrel_landed:;

        int lvl = (*it)->get_level();
        if (lvl >= 0 && lvl < (int)platforms.size()) {
            float bx = (*it)->get_pos().x;
            bool at_edge = (lvl % 2 == 0) ? (bx > 740) : (bx < 60);
            if (at_edge && lvl < (int)platforms.size() - 1) {
                int next = lvl + 1;
                float next_y = platforms[next].get_bounds().position.y;
                float new_x = (next % 2 == 0) ? 60.f : 728.f;
                float new_vx = (next % 2 == 0) ? 180.f : -180.f;
                (*it)->set_position(new_x, next_y);
                (*it)->set_velocity(new_vx, 0);
                (*it)->set_level(next);
            }
        }

        if (!(*it)->is_alive())
            it = barrels.erase(it);
        else
            ++it;
    }

    check_collisions();

    // Lava kills
    if (player.get_pos().y > 710 && state == State::Playing) {
        state = State::GameOver;
        player.set_dead(true);
        crowns = 0;
    }

    // Win: player reached top platform near princess
    if (!platforms.empty()) {
        float top_y = platforms[0].get_bounds().position.y;
        if (player.get_pos().y < top_y + 30 && player.get_pos().x < 150 && state == State::Playing) {
            state = State::Won;
            crowns++;
        }
    }
}

void Game::spawn_barrel() {
    if (platforms.empty()) return;
    float y = platforms[0].get_bounds().position.y;
    barrels.push_back(std::make_unique<Barrel>(60.f, y, 180.f, 0));
}

void Game::check_collisions() {
    for (auto& b : barrels) {
        if (player.get_bounds().findIntersection(b->get_bounds()).has_value()) {
            state = State::GameOver;
            player.set_dead(true);
            crowns = 0;
            return;
        }
    }
}

void Game::draw() {
    window.clear(sf::Color{20, 20, 40});

    if (state == State::Menu) {
        window.draw(title_text);
        window.draw(menu_btn);
        window.draw(menu_btn_text);
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

    if (bg_tex.getSize().x > 0) {
        sf::Sprite bg_spr(bg_tex);
        window.draw(bg_spr);
    } else {
        window.draw(bg_shape);
    }

    // Stage color tint overlay (background only)
    {
        sf::Color stage_tints[] = {
            {60, 20, 30, 35},   {20, 50, 40, 35},   {30, 20, 60, 35},
            {60, 50, 10, 35},   {50, 20, 60, 35},   {10, 50, 60, 35},
            {60, 40, 10, 35},   {40, 20, 55, 35}
        };
        sf::RectangleShape tint_overlay({800, 750});
        tint_overlay.setFillColor(stage_tints[(stage - 1) % 8]);
        window.draw(tint_overlay);
    }

    // Animated torches on towers and platforms
    {
        int flicker = int(lava_anim) % 6;
        float fh = 10.f + flicker * 1.5f;
        auto draw_torch = [&](float tx, float ty) {
            sf::RectangleShape post({3, 14});
            post.setFillColor(sf::Color{80, 60, 30});
            post.setPosition({tx - 1, ty - 14});
            window.draw(post);
            sf::RectangleShape flame({8, fh});
            flame.setPosition({tx - 4, ty - 14 - fh});
            sf::Color fc(255, 120 + flicker * 15, 30);
            if (flicker < 2 || flicker > 4) fc = sf::Color(255, 80, 20);
            flame.setFillColor(fc);
            window.draw(flame);
        };
        draw_torch(77, 140);   // left tower
        draw_torch(722, 140);  // right tower
        // Torches on platforms at various positions
        for (size_t pi = 0; pi < platforms.size(); pi++) {
            float py = platforms[pi].get_bounds().position.y;
            draw_torch(100, py);
            draw_torch(700, py);
        }
    }

    // Lava at bottom
    if (lava_tex.getSize().x > 0) {
        sf::Sprite lava_spr(lava_tex);
        lava_spr.setPosition({0.f, 710.f});
        window.draw(lava_spr);
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

    // Platforms with holes
    if (plat_tex.getSize().x > 0) {
        for (int pi = 0; pi < (int)platforms.size(); pi++) {
            auto& p = platforms[pi];
            auto ppos = p.get_bounds().position;
            sf::Sprite p_spr(plat_tex);
            p_spr.setPosition(ppos);
            window.draw(p_spr);
            // Draw holes as dark rectangles
            for (int s = 0; s < 10; s++) {
                if (is_solid(pi, s)) continue;
                sf::RectangleShape hole({70, 14});
                hole.setPosition({ppos.x + s * 70.f, ppos.y});
                hole.setFillColor(sf::Color{5, 5, 15});
                window.draw(hole);
            }
        }
    } else {
        for (int pi = 0; pi < (int)platforms.size(); pi++) {
            auto& p = platforms[pi];
            auto ppos = p.get_bounds().position;
            sf::RectangleShape seg_shape({70, 14});
            seg_shape.setFillColor(sf::Color{139, 69, 19});
            for (int s = 0; s < 10; s++) {
                if (!is_solid(pi, s)) continue;
                seg_shape.setPosition({ppos.x + s * 70.f, ppos.y});
                window.draw(seg_shape);
            }
        }
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
    if (stage == 9) {
        if (princess_tex.getSize().x > 0) {
            sf::Sprite spr(princess_tex);
            spr.setPosition({70, 104});
            window.draw(spr);
        } else
            window.draw(princess);
    }
    player.draw(window);

    for (auto& b : barrels)
        b->draw(window);

    // Stage number (top-right)
    std::string sstr = std::to_string(stage);
    crowns_text.setString(sstr);
    crowns_text.setPosition({770, 10});
    crowns_text.setOrigin({crowns_text.getLocalBounds().size.x, 0});
    window.draw(crowns_text);

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
        if (crowns >= 9) {
            status_text.setString("CONGRATULATIONS!");
            status_text.setPosition({200, 160});
            window.draw(status_text);
            sf::Text congrats_text(font);
            congrats_text.setString("You completed the Donkey Kong challenge!");
            congrats_text.setCharacterSize(22);
            congrats_text.setFillColor(sf::Color::White);
            auto ct = congrats_text.getLocalBounds();
            congrats_text.setOrigin({ct.position.x + ct.size.x / 2, ct.position.y + ct.size.y / 2});
            congrats_text.setPosition({400, 230});
            window.draw(congrats_text);
            std::string cstr = "CROWNS: " + std::to_string(crowns);
            crowns_text.setString(cstr);
            crowns_text.setPosition({400, 280});
            crowns_text.setOrigin({crowns_text.getLocalBounds().size.x / 2, 0});
            window.draw(crowns_text);
            menu_btn_text.setString("PLAY AGAIN");
        } else {
            status_text.setString("YOU WIN!");
            status_text.setPosition({300, 200});
            window.draw(status_text);
            std::string cstr = "CROWNS: " + std::to_string(crowns);
            crowns_text.setString(cstr);
            crowns_text.setPosition({400, 260});
            crowns_text.setOrigin({crowns_text.getLocalBounds().size.x / 2, 0});
            window.draw(crowns_text);
            menu_btn_text.setString("NEXT STAGE");
        }
        menu_btn.setPosition({400, 340});
        auto mt = menu_btn_text.getLocalBounds();
        menu_btn_text.setOrigin({mt.position.x + mt.size.x / 2, mt.position.y + mt.size.y / 2});
        menu_btn_text.setPosition({400, 340});
        window.draw(menu_btn);
        window.draw(menu_btn_text);
    }

    window.display();
}
