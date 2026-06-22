#include "GameView.hpp"
#include "../model/GameState.hpp"
#include "../model/Player.hpp"
#include "../model/Platform.hpp"
#include "../model/Barrel.hpp"
#include "../model/DonkeyKong.hpp"
#include <cmath>

// ── Layout constants for title screen ──
static sf::FloatRect r_easy()   { return {{160, 260}, {130, 50}}; }
static sf::FloatRect r_normal() { return {{335, 260}, {130, 50}}; }
static sf::FloatRect r_hard()   { return {{510, 260}, {130, 50}}; }
static sf::FloatRect r_custom() { return {{320, 340}, {160, 50}}; }
static sf::FloatRect r_play_t() { return {{320, 430}, {160, 50}}; }

// ── Layout constants for custom screen ──
static sf::FloatRect r_speed()   { return {{250, 230}, {300, 20}}; }
static sf::FloatRect r_interval(){ return {{250, 350}, {300, 20}}; }
static sf::FloatRect r_cplay()   { return {{320, 420}, {160, 50}}; }

sf::FloatRect GameView::title_btn_easy()   { return r_easy(); }
sf::FloatRect GameView::title_btn_normal() { return r_normal(); }
sf::FloatRect GameView::title_btn_hard()   { return r_hard(); }
sf::FloatRect GameView::title_btn_custom() { return r_custom(); }
sf::FloatRect GameView::title_btn_play()   { return r_play_t(); }
sf::FloatRect GameView::custom_speed_track()   { return r_speed(); }
sf::FloatRect GameView::custom_interval_track(){ return r_interval(); }
sf::FloatRect GameView::custom_btn_play()      { return r_cplay(); }

GameView::GameView()
    : window(sf::VideoMode({800u, 750u}), "Donkey Kong") {

    window.setFramerateLimit(60);
    load_font();

    title_text.setString("DONKEY KONG");
    title_text.setCharacterSize(48);
    title_text.setFillColor(sf::Color::Yellow);
    auto tb = title_text.getLocalBounds();
    title_text.setOrigin({tb.position.x + tb.size.x / 2, tb.position.y + tb.size.y / 2});
    title_text.setPosition({400, 100});

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
    setup_btn(pause_resume_btn, pause_resume_text, 400, 260, "RESUME");
    setup_btn(pause_reset_btn, pause_reset_text, 400, 340, "MENU");

    princess.setSize({20, 36});
    princess.setFillColor(sf::Color::Magenta);
    princess.setPosition({70, 104});
    [[maybe_unused]] bool pt_loaded = princess_tex.loadFromFile("assets/sprites/princess.png");

    crowns_text.setCharacterSize(24);
    crowns_text.setFillColor(sf::Color::Yellow);
    [[maybe_unused]] bool crown_loaded = crown_tex.loadFromFile("assets/sprites/crown.png");

    stage_text.setCharacterSize(18);
    stage_text.setFillColor(sf::Color{180, 180, 200});

    mute_btn.setSize({22, 22});
    mute_btn.setFillColor(sf::Color{80, 80, 80, 180});
    mute_btn.setPosition({8, 10});
    mute_text.setString("M");
    mute_text.setCharacterSize(14);
    mute_text.setFillColor(sf::Color::White);
    auto mb = mute_text.getLocalBounds();
    mute_text.setOrigin({mb.position.x + mb.size.x / 2, mb.position.y + mb.size.y / 2});
    mute_text.setPosition({19, 21});

    time_text.setCharacterSize(14);
    time_text.setFillColor(sf::Color::White);
    record_text.setCharacterSize(14);
    record_text.setFillColor(sf::Color::White);
    overall_rec_text.setCharacterSize(14);
    overall_rec_text.setFillColor(sf::Color::Yellow);
    stage_rec_text.setCharacterSize(14);
    stage_rec_text.setFillColor(sf::Color::Yellow);

    [[maybe_unused]] bool bg_ok = bg_tex.loadFromFile("assets/sprites/background.png");
    [[maybe_unused]] bool p_ok = plat_tex.loadFromFile("assets/sprites/platform.png");
    [[maybe_unused]] bool lt_ok = ladder_tex.loadFromFile("assets/sprites/ladder.png");
    [[maybe_unused]] bool lv_ok = lava_tex.loadFromFile("assets/sprites/lava.png");
    bg_shape.setFillColor(sf::Color{20, 20, 40});
}

bool GameView::load_font() {
    if (font.openFromFile("assets/fonts/arial.ttf")) return true;
    if (font.openFromFile("assets/fonts/DejaVuSans.ttf")) return true;
    if (font.openFromFile("/usr/share/fonts/TTF/DejaVuSans.ttf")) return true;
    if (font.openFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) return true;
    if (font.openFromFile("/usr/share/fonts/Adwaita/AdwaitaSans-Regular.ttf")) return true;
    return false;
}

sf::FloatRect GameView::get_menu_btn_bounds() const {
    return menu_btn.getGlobalBounds();
}

sf::FloatRect GameView::get_pause_resume_btn_bounds() const {
    return pause_resume_btn.getGlobalBounds();
}

sf::FloatRect GameView::get_pause_reset_btn_bounds() const {
    return pause_reset_btn.getGlobalBounds();
}

sf::FloatRect GameView::get_mute_btn_bounds() const {
    return mute_btn.getGlobalBounds();
}

sf::FloatRect GameView::draw_btn(sf::FloatRect rect, const std::string& label, const sf::Color& color) {
    sf::RectangleShape btn({rect.size.x, rect.size.y});
    btn.setPosition({rect.position.x, rect.position.y});
    btn.setFillColor(color);
    window.draw(btn);
    sf::Text txt(font, label, 24);
    txt.setFillColor(sf::Color::White);
    auto tb = txt.getLocalBounds();
    txt.setOrigin({tb.position.x + tb.size.x / 2, tb.position.y + tb.size.y / 2});
    txt.setPosition({rect.position.x + rect.size.x / 2, rect.position.y + rect.size.y / 2});
    window.draw(txt);
    return rect;
}

float GameView::draw_slider(float cx, float cy, float min_val, float max_val, float val) {
    float track_w = 300.f;
    float norm = (val - min_val) / (max_val - min_val);
    float hx = cx - track_w / 2.f + norm * track_w;

    sf::RectangleShape track({track_w, 6.f});
    track.setOrigin({track_w / 2.f, 3.f});
    track.setPosition({cx, cy});
    track.setFillColor(sf::Color{100, 100, 120});
    window.draw(track);

    sf::CircleShape handle(8.f);
    handle.setOrigin({8.f, 8.f});
    handle.setPosition({hx, cy});
    handle.setFillColor(sf::Color{200, 200, 100});
    handle.setOutlineColor(sf::Color::White);
    handle.setOutlineThickness(2.f);
    window.draw(handle);

    return hx;
}

void GameView::draw(const GameState& state, const Player& player,
                    const std::vector<Platform>& platforms,
                    const std::vector<Ladder>& ladders,
                    const std::vector<std::unique_ptr<Barrel>>& barrels,
                    const DonkeyKong& dk, float lava_anim) {

    window.clear(sf::Color{20, 20, 40});

    // ── TITLE SCREEN ──
    if (state.phase == GameState::Phase::Title) {
        sf::RectangleShape overlay({800, 750});
        overlay.setFillColor(sf::Color{0, 0, 0, 180});
        window.draw(overlay);
        window.draw(title_text);

        // Crown display
        std::string cstr = std::to_string(state.crowns);
        crowns_text.setString(cstr);
        crowns_text.setPosition({400, 170});
        crowns_text.setOrigin({crowns_text.getLocalBounds().size.x / 2, 0});
        window.draw(crowns_text);
        if (crown_tex.getSize().x > 0) {
            sf::Sprite crown_spr(crown_tex);
            crown_spr.setOrigin({16, 16});
            crown_spr.setPosition({400 - crowns_text.getLocalBounds().size.x / 2 - 24, 174});
            window.draw(crown_spr);
        }

        // Difficulty buttons
        auto is_sel = [&](GameState::Difficulty d) { return state.difficulty == d; };
        auto diff_color = [&](GameState::Difficulty d) {
            return is_sel(d) ? sf::Color{80, 180, 80} : sf::Color{60, 60, 140};
        };
        draw_btn(r_easy(),   "EASY",   diff_color(GameState::Difficulty::Easy));
        draw_btn(r_normal(), "NORMAL", diff_color(GameState::Difficulty::Normal));
        draw_btn(r_hard(),   "HARD",   diff_color(GameState::Difficulty::Hard));
        draw_btn(r_custom(), "CUSTOM", diff_color(GameState::Difficulty::Custom));
        draw_btn(r_play_t(), "PLAY",   sf::Color{50, 150, 50});

        window.display();
        return;
    }

    // ── CUSTOM DIFFICULTY SCREEN ──
    if (state.phase == GameState::Phase::Custom) {
        sf::RectangleShape overlay({800, 750});
        overlay.setFillColor(sf::Color{0, 0, 0, 200});
        window.draw(overlay);

        sf::Text heading(font, "CUSTOM DIFFICULTY", 32);
        heading.setFillColor(sf::Color::Yellow);
        auto hb = heading.getLocalBounds();
        heading.setOrigin({hb.position.x + hb.size.x / 2, hb.position.y + hb.size.y / 2});
        heading.setPosition({400, 100});
        window.draw(heading);

        // Speed label + slider
        {
            sf::Text lbl(font, "Barrel Speed:", 20);
            lbl.setFillColor(sf::Color::White);
            auto lb = lbl.getLocalBounds();
            lbl.setOrigin({lb.position.x + lb.size.x / 2, 0});
            lbl.setPosition({400, 170});
            window.draw(lbl);
            draw_slider(400, 240, 60.f, 400.f, state.custom_speed);
            sf::Text val(font, std::to_string(int(state.custom_speed)), 18);
            val.setFillColor(sf::Color{200, 200, 100});
            auto vb = val.getLocalBounds();
            val.setOrigin({vb.position.x + vb.size.x / 2, 0});
            val.setPosition({400, 260});
            window.draw(val);
        }

        // Interval label + slider
        {
            sf::Text lbl(font, "Barrel Rate (seconds):", 20);
            lbl.setFillColor(sf::Color::White);
            auto lb = lbl.getLocalBounds();
            lbl.setOrigin({lb.position.x + lb.size.x / 2, 0});
            lbl.setPosition({400, 290});
            window.draw(lbl);
            draw_slider(400, 360, 0.5f, 5.f, state.custom_interval);
            char ibuf[16];
            std::snprintf(ibuf, sizeof(ibuf), "%.1f s", (double)state.custom_interval);
            sf::Text val(font, ibuf, 18);
            val.setFillColor(sf::Color{200, 200, 100});
            auto vb2 = val.getLocalBounds();
            val.setOrigin({vb2.position.x + vb2.size.x / 2, 0});
            val.setPosition({400, 380});
            window.draw(val);
        }

        draw_btn(r_cplay(), "PLAY", sf::Color{50, 150, 50});

        window.display();
        return;
    }

    // ── GAME BACKGROUND ──
    if (bg_tex.getSize().x > 0) {
        sf::Sprite bg_spr(bg_tex);
        window.draw(bg_spr);
    } else {
        window.draw(bg_shape);
    }

    // Stage color tint overlay
    {
        sf::Color stage_tints[] = {
            {60, 20, 30, 35},   {20, 50, 40, 35},   {30, 20, 60, 35},
            {60, 50, 10, 35},   {50, 20, 60, 35},   {10, 50, 60, 35},
            {60, 40, 10, 35},   {40, 20, 55, 35}
        };
        sf::RectangleShape tint_overlay({800, 750});
        tint_overlay.setFillColor(stage_tints[(state.stage - 1) % 8]);
        window.draw(tint_overlay);
    }

    // Animated torches
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
        draw_torch(77, 140);
        draw_torch(722, 140);
        for (size_t pi = 0; pi < platforms.size(); pi++) {
            float py = platforms[pi].get_bounds().position.y;
            draw_torch(100, py);
            draw_torch(700, py);
        }
    }

    // Lava
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
    auto is_solid = [&](int pi, int s) {
        return pi < (int)platforms.size() && s < 10 && platforms[pi].is_solid(s);
    };

    if (plat_tex.getSize().x > 0) {
        for (int pi = 0; pi < (int)platforms.size(); pi++) {
            auto& p = platforms[pi];
            auto ppos = p.get_bounds().position;
            sf::Sprite p_spr(plat_tex);
            p_spr.setPosition(ppos);
            window.draw(p_spr);
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
    if (state.stage == 9) {
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
    std::string sstr = std::to_string(state.stage);
    crowns_text.setString(sstr);
    crowns_text.setOrigin({crowns_text.getLocalBounds().size.x, 0});
    crowns_text.setPosition({770, 10});
    window.draw(crowns_text);

    // Mute button
    window.draw(mute_btn);
    window.draw(mute_text);

    // Lives display at bottom-center
    {
        int max_lives = 3;
        float spacing = 22.f;
        float start_x = 400 - (max_lives - 1) * spacing * 0.5f;
        float hy = 725.f;
        for (int i = 0; i < max_lives; i++) {
            float hx = start_x + i * spacing;
            sf::Color hc = i < state.lives ? sf::Color::Red : sf::Color{60, 60, 60};
            sf::CircleShape c(4.f);
            c.setOrigin({4, 4});
            c.setFillColor(hc);
            c.setOutlineColor(sf::Color::Black);
            c.setOutlineThickness(1.f);
            c.setPosition({hx - 4, hy - 2});
            window.draw(c);
            c.setPosition({hx + 4, hy - 2});
            window.draw(c);
            sf::ConvexShape tri(3);
            tri.setPoint(0, {hx - 5, hy + 1});
            tri.setPoint(1, {hx + 5, hy + 1});
            tri.setPoint(2, {hx, hy + 7});
            tri.setFillColor(hc);
            tri.setOutlineColor(sf::Color::Black);
            tri.setOutlineThickness(1.f);
            window.draw(tri);
        }
    }

    // Timer display (middle-top)
    {
        time_text.setString("OVERALL  " + GameState::fmt_time(state.overall_timer));
        auto ot = time_text.getLocalBounds();
        time_text.setOrigin({ot.size.x / 2, 0});
        time_text.setPosition({400, 8});
        window.draw(time_text);

        float orec = state.records.overall_time;
        if (orec >= 0) {
            overall_rec_text.setString("RECORD   " + GameState::fmt_time(orec));
            auto ort = overall_rec_text.getLocalBounds();
            overall_rec_text.setOrigin({ort.size.x / 2, 0});
            overall_rec_text.setPosition({400, 24});
            window.draw(overall_rec_text);
        }

        record_text.setString("STAGE    " + GameState::fmt_time(state.stage_timer));
        auto st = record_text.getLocalBounds();
        record_text.setOrigin({st.size.x / 2, 0});
        record_text.setPosition({400, 44});
        window.draw(record_text);

        float srec = state.records.stage_times[state.stage - 1];
        if (srec >= 0) {
            stage_rec_text.setString("RECORD   " + GameState::fmt_time(srec));
            auto srt = stage_rec_text.getLocalBounds();
            stage_rec_text.setOrigin({srt.size.x / 2, 0});
            stage_rec_text.setPosition({400, 60});
            window.draw(stage_rec_text);
        }
    }

    // ── PAUSED OVERLAY ──
    if (state.paused) {
        sf::RectangleShape overlay({800, 750});
        overlay.setFillColor(sf::Color{0, 0, 0, 150});
        window.draw(overlay);

        status_text.setString("PAUSED");
        auto stp = status_text.getLocalBounds();
        status_text.setOrigin({stp.size.x / 2, stp.size.y / 2});
        status_text.setPosition({400, 180});
        window.draw(status_text);

        window.draw(pause_resume_btn);
        window.draw(pause_resume_text);
        window.draw(pause_reset_btn);
        window.draw(pause_reset_text);
    }

    // ── GAME OVER OVERLAY ──
    if (state.phase == GameState::Phase::GameOver) {
        sf::RectangleShape overlay({800, 750});
        overlay.setFillColor(sf::Color{0, 0, 0, 150});
        window.draw(overlay);
        status_text.setString("GAME OVER");
        auto stg = status_text.getLocalBounds();
        status_text.setOrigin({stg.size.x / 2, stg.size.y / 2});
        status_text.setPosition({400, 250});
        window.draw(status_text);
        menu_btn.setPosition({400, 340});
        menu_btn_text.setString("MENU");
        auto mt = menu_btn_text.getLocalBounds();
        menu_btn_text.setOrigin({mt.position.x + mt.size.x / 2, mt.position.y + mt.size.y / 2});
        menu_btn_text.setPosition({400, 340});
        window.draw(menu_btn);
        window.draw(menu_btn_text);
    }

    // ── STAGE COMPLETE / WON OVERLAY ──
    if (state.phase == GameState::Phase::Won) {
        sf::RectangleShape overlay({800, 750});
        overlay.setFillColor(sf::Color{0, 0, 0, 150});
        window.draw(overlay);
        int si = state.stage - 1;
        float prev_stage = state.records.stage_times[si];
        bool new_stage_rec = (state.stage_timer < prev_stage || prev_stage < 0);
        bool new_overall_rec = false;
        if (state.crowns >= 9 && (state.overall_timer < state.records.overall_time || state.records.overall_time < 0))
            new_overall_rec = true;

        if (state.crowns >= 9)
            status_text.setString("CONGRATULATIONS!");
        else
            status_text.setString("STAGE COMPLETE!");
        auto stb = status_text.getLocalBounds();
        status_text.setOrigin({stb.size.x / 2, 0});
        status_text.setPosition({400, 120});
        window.draw(status_text);

        sf::Text st_time(font);
        st_time.setCharacterSize(18);
        std::string st_line = "STAGE TIME:   " + GameState::fmt_time(state.stage_timer);
        if (new_stage_rec) st_line += "   NEW RECORD!";
        st_time.setString(st_line);
        st_time.setFillColor(new_stage_rec ? sf::Color::Yellow : sf::Color::White);
        auto stt = st_time.getLocalBounds();
        st_time.setOrigin({stt.size.x / 2, 0});
        st_time.setPosition({400, 180});
        window.draw(st_time);

        if (prev_stage >= 0) {
            sf::Text prev_st(font);
            prev_st.setString("BEST:         " + GameState::fmt_time(prev_stage));
            prev_st.setCharacterSize(18);
            prev_st.setFillColor(sf::Color{180, 180, 200});
            auto pst = prev_st.getLocalBounds();
            prev_st.setOrigin({pst.size.x / 2, 0});
            prev_st.setPosition({400, 205});
            window.draw(prev_st);
        }

        if (state.crowns >= 9) {
            sf::Text ov_text(font);
            ov_text.setCharacterSize(18);
            std::string ov_line = "OVERALL TIME: " + GameState::fmt_time(state.overall_timer);
            if (new_overall_rec) ov_line += "   NEW RECORD!";
            ov_text.setString(ov_line);
            ov_text.setFillColor(new_overall_rec ? sf::Color::Yellow : sf::Color::White);
            auto ovt = ov_text.getLocalBounds();
            ov_text.setOrigin({ovt.size.x / 2, 0});
            ov_text.setPosition({400, 240});
            window.draw(ov_text);

            if (state.records.overall_time >= 0) {
                sf::Text prev_ov(font);
                prev_ov.setString("BEST:         " + GameState::fmt_time(state.records.overall_time));
                prev_ov.setCharacterSize(18);
                prev_ov.setFillColor(sf::Color{180, 180, 200});
                auto povt = prev_ov.getLocalBounds();
                prev_ov.setOrigin({povt.size.x / 2, 0});
                prev_ov.setPosition({400, 265});
                window.draw(prev_ov);
            }
        } else {
            sf::Text ov_text(font);
            ov_text.setCharacterSize(18);
            ov_text.setString("OVERALL: " + GameState::fmt_time(state.overall_timer));
            ov_text.setFillColor(sf::Color::White);
            auto ovt = ov_text.getLocalBounds();
            ov_text.setOrigin({ovt.size.x / 2, 0});
            ov_text.setPosition({400, 240});
            window.draw(ov_text);
        }

        std::string sstr2 = std::to_string(state.stage);
        stage_text.setString(sstr2);
        stage_text.setPosition({770, 10});
        stage_text.setOrigin({stage_text.getLocalBounds().size.x, 0});
        window.draw(stage_text);

        float bx[] = {300, 500};
        const char* labels[] = {
            state.crowns >= 9 ? "PLAY AGAIN" : "NEXT STAGE",
            "MENU"
        };
        sf::RectangleShape* btns[] = {&menu_btn, &pause_reset_btn};
        sf::Text* btn_texts[] = {&menu_btn_text, &pause_reset_text};
        for (int bi = 0; bi < 2; bi++) {
            btns[bi]->setSize({160, 50});
            btns[bi]->setOrigin({80, 25});
            btns[bi]->setPosition({bx[bi], 340});
            btns[bi]->setFillColor(sf::Color{60, 60, 180});
            window.draw(*btns[bi]);
            btn_texts[bi]->setString(labels[bi]);
            btn_texts[bi]->setCharacterSize(24);
            btn_texts[bi]->setFillColor(sf::Color::White);
            auto tb = btn_texts[bi]->getLocalBounds();
            btn_texts[bi]->setOrigin({tb.position.x + tb.size.x / 2, tb.position.y + tb.size.y / 2});
            btn_texts[bi]->setPosition({bx[bi], 340});
            window.draw(*btn_texts[bi]);
        }
    }

    window.display();
}
