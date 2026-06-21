#include <SFML/Graphics.hpp>
#include <filesystem>
#include <iostream>
#include <cmath>

struct Pos { int x, y; };
static auto sp(auto& img, Pos p, sf::Color c) { img.setPixel({(unsigned)p.x, (unsigned)p.y}, c); }
static auto C(int r, int g, int b) { return sf::Color(r, g, b); }

// Player: sprite sheet 72x32 (three 24x32 frames: stand, walk, death)
static sf::Image make_player() {
    sf::Image img;
    img.resize({72, 32}, sf::Color::Transparent);
    auto p = [&](int x, int y, sf::Color c) { sp(img, {x, y}, c); };
    auto fill = [&](int x1, int y1, int x2, int y2, sf::Color c) {
        for (int y = y1; y < y2; y++) for (int x = x1; x < x2; x++) p(x, y, c);
    };
    for (int f = 0; f < 3; f++) {
        int ox = f * 24;
        bool death = (f == 2);
        // Red cap (slightly tilted in death)
        int ct = death ? 1 : 0;
        fill(ox + 5 + ct, 0, ox + 19 + ct, 3, C(220, 40, 40));
        fill(ox + 7 + ct, 3, ox + 17 + ct, 4, C(220, 40, 40));
        fill(ox + 3 + ct, 4, ox + 21 + ct, 5, death ? C(130, 20, 20) : C(190, 30, 30));
        // Face
        fill(ox + 6, 5, ox + 18, 10, death ? C(200, 180, 160) : C(250, 215, 185));
        // Eyes (X eyes for death, normal otherwise)
        if (death) {
            p(ox + 8, 6, C(255,50,50)); p(ox + 10, 6, C(255,50,50));
            p(ox + 9, 5, C(255,50,50)); p(ox + 9, 7, C(255,50,50));
            p(ox + 13, 6, C(255,50,50)); p(ox + 15, 6, C(255,50,50));
            p(ox + 14, 5, C(255,50,50)); p(ox + 14, 7, C(255,50,50));
            // Open mouth
            fill(ox + 10, 8, ox + 14, 10, C(100, 40, 40));
        } else {
            p(ox + 9, 6, C(0,0,0)); p(ox + 14, 6, C(0,0,0));
            p(ox + 8, 6, C(255,255,255)); p(ox + 13, 6, C(255,255,255));
            fill(ox + 10, 8, ox + 14, 9, C(180, 100, 80));
        }
        // Body (red shirt)
        fill(ox + 5, 10, ox + 19, 18, death ? C(160, 30, 30) : C(200, 40, 40));
        // Blue overalls
        fill(ox + 5, 16, ox + 19, 22, death ? C(35, 60, 160) : C(50, 90, 210));
        // Straps
        fill(ox + 7, 13, ox + 10, 16, death ? C(35, 60, 160) : C(50, 90, 210));
        fill(ox + 14, 13, ox + 17, 16, death ? C(35, 60, 160) : C(50, 90, 210));
        // Belt
        fill(ox + 6, 20, ox + 18, 21, death ? C(140, 120, 40) : C(180, 160, 60));
        // Arms (death: arms raised up)
        if (death) {
            fill(ox + 2, 8, ox + 5, 13, C(200, 180, 160));
            fill(ox + 19, 8, ox + 22, 13, C(200, 180, 160));
            fill(ox + 1, 12, ox + 3, 14, C(200, 180, 160));
            fill(ox + 21, 12, ox + 23, 14, C(200, 180, 160));
        } else {
            fill(ox + 2 - (f==1?1:0), 11, ox + 5 - (f==1?1:0), 17, C(250, 215, 185));
            fill(ox + 19 + (f==1?1:0), 11, ox + 22 + (f==1?1:0), 17, C(250, 215, 185));
            fill(ox + 1 - (f==1?1:0), 15, ox + 3 - (f==1?1:0), 17, C(250, 215, 185));
            fill(ox + 20 + (f==1?1:0), 15, ox + 22 + (f==1?1:0), 17, C(250, 215, 185));
        }
        // Legs (death: legs slightly apart)
        int s = death ? 2 : ((f == 1) ? 4 : 0);
        fill(ox + 6 - s, 22, ox + 12 - s, 29, death ? C(30, 50, 130) : C(40, 70, 180));
        fill(ox + 12 + s, 22, ox + 18 + s, 29, death ? C(30, 50, 130) : C(40, 70, 180));
        // Shoes
        fill(ox + 5 - s, 28, ox + 13 - s, 32, death ? C(70, 40, 15) : C(90, 55, 25));
        fill(ox + 11 + s, 28, ox + 19 + s, 32, death ? C(70, 40, 15) : C(90, 55, 25));
    }
    return img;
}

// Barrel: 24x24 red barrel with metal bands
static sf::Image make_barrel() {
    sf::Image img;
    img.resize({24, 24}, sf::Color::Transparent);
    auto p = [&](int x, int y, sf::Color c) { sp(img, {x, y}, c); };
    auto fill = [&](int x1, int y1, int x2, int y2, sf::Color c) {
        for (int y = y1; y < y2; y++) for (int x = x1; x < x2; x++) if ((x-12)*(x-12)+(y-12)*(y-12) <= 144) p(x, y, c);
    };
    // Red base
    for (int y = 0; y < 24; y++) for (int x = 0; x < 24; x++) {
        float dx = x - 12, dy = y - 12;
        if (dx * dx + dy * dy <= 144)
            p(x, y, C(210, 50, 50));
    }
    // Darker bands
    fill(0, 5, 24, 8, C(160, 150, 130));
    fill(0, 16, 24, 19, C(160, 150, 130));
    // Band rivets
    for (int x = 5; x < 20; x += 7) { p(x, 6, C(200, 190, 170)); p(x+1, 6, C(200,190,170)); p(x, 7, C(200,190,170)); p(x+1,7,C(200,190,170)); }
    for (int x = 5; x < 20; x += 7) { p(x, 17, C(200,190,170)); p(x+1, 17, C(200,190,170)); p(x, 18, C(200,190,170)); p(x+1,18,C(200,190,170)); }
    // Arrow marker for rotation
    fill(10, 1, 14, 4, C(255, 200, 50));
    p(11, 0, C(255,200,50)); p(12, 0, C(255,200,50));
    // Highlight
    fill(4, 2, 9, 6, C(240, 100, 100));
    return img;
}

// DK: 48x56 (smaller hitbox)
static sf::Image make_donkey_kong() {
    sf::Image img;
    img.resize({48, 56}, sf::Color::Transparent);
    auto p = [&](int x, int y, sf::Color c) { sp(img, {x, y}, c); };
    auto fill = [&](int x1, int y1, int x2, int y2, sf::Color c) {
        for (int y = y1; y < y2; y++) for (int x = x1; x < x2; x++) p(x, y, c);
    };
    // Body (brown)
    fill(10, 16, 38, 44, C(120, 80, 40));
    // Belly (lighter)
    fill(16, 20, 32, 38, C(160, 120, 60));
    // Head
    fill(12, 2, 36, 18, C(130, 90, 50));
    // Ears
    fill(5, 4, 12, 10, C(100, 60, 30));
    fill(36, 4, 43, 10, C(100, 60, 30));
    // Inner ears
    fill(7, 5, 11, 9, C(160, 100, 60));
    fill(37, 5, 41, 9, C(160, 100, 60));
    // Eyes (white)
    fill(16, 10, 21, 13, C(255, 255, 255));
    fill(27, 10, 32, 13, C(255, 255, 255));
    // Pupils
    fill(18, 11, 20, 13, C(0, 0, 0));
    fill(28, 11, 30, 13, C(0, 0, 0));
    // Eyebrows
    fill(15, 8, 22, 10, C(80, 40, 20));
    fill(26, 8, 33, 10, C(80, 40, 20));
    // Nose
    fill(22, 14, 26, 16, C(80, 40, 20));
    // Mouth
    fill(18, 17, 30, 18, C(80, 40, 20));
    // Red tie
    fill(22, 18, 26, 24, C(200, 50, 50));
    fill(23, 24, 25, 27, C(200, 50, 50));
    // Arms
    fill(4, 18, 10, 30, C(110, 70, 35));
    fill(38, 18, 44, 30, C(110, 70, 35));
    // Hands
    fill(3, 26, 10, 30, C(80, 50, 25));
    fill(38, 26, 45, 30, C(80, 50, 25));
    // Legs
    fill(12, 42, 20, 56, C(100, 60, 30));
    fill(28, 42, 36, 56, C(100, 60, 30));
    // Feet
    fill(8, 51, 22, 56, C(80, 40, 20));
    fill(26, 51, 40, 56, C(80, 40, 20));
    // Belly button
    fill(23, 28, 25, 30, C(120, 80, 40));
    return img;
}

// Princess: 20x36
static sf::Image make_princess() {
    sf::Image img;
    img.resize({20, 36}, sf::Color::Transparent);
    auto p = [&](int x, int y, sf::Color c) { sp(img, {x, y}, c); };
    // Hair
    for (int y = 0; y < 10; y++)
        for (int x = 2; x < 18; x++) p(x, y, C(240, 200, 60));
    for (int y = 0; y < 14; y++) {
        for (int x = 0; x < 4; x++) p(x, y, C(240, 200, 60));
        for (int x = 16; x < 20; x++) p(x, y, C(240, 200, 60));
    }
    // Face
    for (int y = 10; y < 16; y++)
        for (int x = 4; x < 16; x++) p(x, y, C(245, 210, 180));
    p(7, 12, C(0,0,0)); p(12, 12, C(0,0,0));
    // Blush
    p(5, 14, C(255, 180, 180)); p(14, 14, C(255, 180, 180));
    // Dress
    for (int y = 16; y < 28; y++)
        for (int x = 3; x < 17; x++) p(x, y, C(255, 80, 160));
    for (int y = 26; y < 36; y++)
        for (int x = 1; x < 19; x++) p(x, y, C(255, 60, 140));
    // Crown
    for (int y = 0; y < 3; y++)
        for (int x = 6; x < 14; x++) p(x, y, C(255, 215, 0));
    // Crown jewels
    p(8, 1, C(255, 0, 0)); p(11, 1, C(0, 150, 255));
    return img;
}

// Crown icon: 32x32
static sf::Image make_crown() {
    sf::Image img;
    img.resize({32, 32}, sf::Color::Transparent);
    auto p = [&](int x, int y, sf::Color c) { sp(img, {x, y}, c); };
    // Gold base
    for (int y = 12; y < 28; y++)
        for (int x = 4; x < 28; x++) p(x, y, C(255, 215, 0));
    // Crown points (3 peaks)
    auto peak = [&](int cx) {
        for (int dy = 0; dy < 12; dy++)
            for (int dx = -3; dx <= 3; dx++)
                if (std::abs(dx) <= dy / 2 + 1)
                    p(cx + dx, 12 - dy, C(255, 215, 0));
    };
    peak(8); peak(16); peak(24);
    // Base outline
    for (int x = 4; x < 28; x++) { p(x, 26, C(180, 150, 0)); p(x, 27, C(180, 150, 0)); }
    // Gems
    p(8, 18, C(255, 0, 0)); p(9, 18, C(255, 0, 0));
    p(16, 16, C(0, 150, 255)); p(17, 16, C(0, 150, 255));
    p(22, 18, C(50, 200, 50)); p(23, 18, C(50, 200, 50));
    // Dark outline
    for (int y = 12; y < 28; y++) {
        p(3, y, C(180, 150, 0)); p(28, y, C(180, 150, 0));
    }
    return img;
}

// Background: 800x750 dark with support structures
static sf::Image make_background() {
    sf::Image img;
    img.resize({800, 750}, sf::Color::Transparent);
    auto p = [&](int x, int y, sf::Color c) { sp(img, {x, y}, c); };
    auto fill = [&](int x1, int y1, int x2, int y2, sf::Color c) {
        for (int y = y1; y < y2; y++) for (int x = x1; x < x2; x++) p(x, y, c);
    };
    // Dark gradient background
    for (int y = 0; y < 750; y++) {
        int b = 8 + y / 50;
        if (b > 25) b = 25;
        sf::Color c(3, 3, b);
        for (int x = 0; x < 800; x++) img.setPixel({(unsigned)x, (unsigned)y}, c);
    }
    // Sparse dim stars
    auto star = [&](int sx, int sy) {
        for (int dy = -1; dy <= 1; dy++)
            for (int dx = -1; dx <= 1; dx++)
                if (std::abs(dx) + std::abs(dy) <= 1)
                    img.setPixel({(unsigned)(sx + dx), (unsigned)(sy + dy)}, sf::Color(160, 170, 200, 100));
    };
    star(120, 40); star(340, 70); star(560, 25); star(720, 55);
    star(200, 100); star(480, 45); star(650, 85); star(80, 90);

    // Support structures: left & right steel columns connecting platforms
    sf::Color steel(50, 55, 65);
    sf::Color steel_hl(70, 75, 85);
    // Left column (x=44..56)
    fill(44, 140, 56, 710, steel);
    fill(44, 140, 47, 710, steel_hl);
    // Right column (x=744..756)
    fill(744, 140, 756, 710, steel);
    fill(753, 140, 756, 710, steel_hl);
    // Horizontal beams at each platform level
    float levels[] = {140.f, 254.f, 368.f, 482.f, 596.f, 710.f};
    for (float ly : levels) {
        int y = int(ly);
        // Beam across bottom of each platform
        fill(44, y + 10, 56, y + 16, C(70, 75, 85));
        fill(744, y + 10, 756, y + 16, C(70, 75, 85));
        // Cross beam connectors
        fill(50, y + 10, 56, y + 14, C(90, 95, 105));
        fill(744, y + 10, 750, y + 14, C(90, 95, 105));
    }
    // Diagonal cross braces between levels
    auto cross = [&](int x1, int y1, int x2, int y2, sf::Color c) {
        int dx = x2 - x1, dy = y2 - y1;
        int steps = std::max(std::abs(dx), std::abs(dy));
        for (int i = 0; i <= steps; i++) {
            int x = x1 + dx * i / steps;
            int y = y1 + dy * i / steps;
            if (x >= 0 && x < 800 && y >= 0 && y < 750)
                img.setPixel({(unsigned)x, (unsigned)y}, c);
        }
    };
    sf::Color brace(45, 50, 60);
    // Left side X-brace between each pair of levels
    for (int i = 0; i < 5; i++) {
        int y_top = int(levels[i]) + 14;
        int y_bot = int(levels[i+1]);
        cross(48, y_top, 52, y_bot, brace);
        cross(52, y_top, 48, y_bot, brace);
    }
    // Right side X-brace
    for (int i = 0; i < 5; i++) {
        int y_top = int(levels[i]) + 14;
        int y_bot = int(levels[i+1]);
        cross(748, y_top, 752, y_bot, brace);
        cross(752, y_top, 748, y_bot, brace);
    }
    return img;
}

// Platform texture: 700x14 brown with wood grain
static sf::Image make_platform_tex() {
    sf::Image img;
    img.resize({700, 14}, sf::Color::Transparent);
    // Base brown
    for (int y = 0; y < 14; y++)
        for (int x = 0; x < 700; x++)
            img.setPixel({(unsigned)x, (unsigned)y}, sf::Color(120, 80, 40));
    // Wood grain lines
    for (int x = 0; x < 700; x++) {
        int g = 20 + (x * 7) % 30;
        img.setPixel({(unsigned)x, 3}, sf::Color(100 + g, 65 + g, 30 + g));
        img.setPixel({(unsigned)x, 7}, sf::Color(95 + g, 60 + g, 25 + g));
        img.setPixel({(unsigned)x, 11}, sf::Color(100 + g, 65 + g, 30 + g));
    }
    // Rivets
    for (int x = 20; x < 700; x += 60) {
        img.setPixel({(unsigned)x, 1}, sf::Color(180, 160, 100));
        img.setPixel({(unsigned)x + 1, 1}, sf::Color(180, 160, 100));
        img.setPixel({(unsigned)x, 2}, sf::Color(180, 160, 100));
        img.setPixel({(unsigned)x + 1, 2}, sf::Color(180, 160, 100));
        img.setPixel({(unsigned)x, 11}, sf::Color(180, 160, 100));
        img.setPixel({(unsigned)x + 1, 11}, sf::Color(180, 160, 100));
        img.setPixel({(unsigned)x, 12}, sf::Color(180, 160, 100));
        img.setPixel({(unsigned)x + 1, 12}, sf::Color(180, 160, 100));
    }
    return img;
}

// Lava: 800x40 animated lava strip
static sf::Image make_lava() {
    sf::Image img;
    img.resize({800, 40}, sf::Color::Transparent);
    auto p = [&](int x, int y, sf::Color c) { sp(img, {x, y}, c); };
    for (int y = 0; y < 40; y++) {
        for (int x = 0; x < 800; x++) {
            float ny = y / 40.f;
            // Glow effect: hot center
            int r = 180 + int(75 * (1 - ny));
            int g = 40 + int(80 * (1 - ny)) - x % 3;
            int b = 5;
            if (r > 255) r = 255;
            if (g > 255) g = 255;
            // Ripple pattern
            int wave = int(4 * (1 - ny) * std::sin(x * 0.08 + y * 0.3));
            if (y > 30 - wave) { r = 255; g = 140; b = 20; }
            p(x, 39 - y, C(r, g, b));
        }
    }
    // Bright surface line
    for (int x = 0; x < 800; x++) {
        p(x, 0, C(255, 200, 50));
        p(x, 1, C(255, 180, 60));
    }
    return img;
}

// Ladder texture: 20x100 with rungs
static sf::Image make_ladder_tex() {
    sf::Image img;
    img.resize({20, 100}, sf::Color::Transparent);
    // Rails (dark brown)
    for (int y = 0; y < 100; y++) {
        for (int x = 0; x < 3; x++) img.setPixel({(unsigned)x, (unsigned)y}, sf::Color(80, 50, 20));
        for (int x = 17; x < 20; x++) img.setPixel({(unsigned)x, (unsigned)y}, sf::Color(80, 50, 20));
    }
    // Rungs (lighter brown)
    for (int r = 0; r < 10; r++) {
        int ry = r * 10 + 2;
        for (int x = 2; x < 18; x++)
            for (int dy = 0; dy < 3; dy++)
                img.setPixel({(unsigned)x, (unsigned)(ry + dy)}, sf::Color(140, 100, 50));
        // Rung highlights
        for (int x = 3; x < 17; x++)
            img.setPixel({(unsigned)x, (unsigned)ry}, sf::Color(160, 120, 60));
    }
    // Rail highlights
    for (int y = 0; y < 100; y++) {
        img.setPixel({0, (unsigned)y}, sf::Color(100, 65, 30));
        img.setPixel({17, (unsigned)y}, sf::Color(100, 65, 30));
    }
    return img;
}

int main() {
    std::filesystem::create_directories("assets/sprites");
    auto save = [](const sf::Image& img, const char* name) {
        std::string path = std::string("assets/sprites/") + name + ".png";
        if (img.saveToFile(path)) std::cout << "Created: " << path << "\n";
        else std::cerr << "Failed: " << path << "\n";
    };
    save(make_player(), "player");
    save(make_barrel(), "barrel");
    save(make_donkey_kong(), "donkey_kong");
    save(make_princess(), "princess");
    save(make_crown(), "crown");
    save(make_background(), "background");
    save(make_platform_tex(), "platform");
    save(make_ladder_tex(), "ladder");
    save(make_lava(), "lava");
    return 0;
}
