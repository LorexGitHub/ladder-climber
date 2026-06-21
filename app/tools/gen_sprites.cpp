#include <SFML/Graphics.hpp>
#include <filesystem>
#include <iostream>
#include <cmath>

struct Pos { int x, y; };
static auto sp(auto& img, Pos p, sf::Color c) { img.setPixel({(unsigned)p.x, (unsigned)p.y}, c); }
static auto C(int r, int g, int b) { return sf::Color(r, g, b); }

// Player: sprite sheet 48x32 (two 24x32 frames: standing, walking)
static sf::Image make_player() {
    sf::Image img;
    img.resize({48, 32}, sf::Color::Transparent);
    auto p = [&](int x, int y, sf::Color c) { sp(img, {x, y}, c); };
    for (int frame = 0; frame < 2; frame++) {
        int ox = frame * 24;
        // Head (skin color)
        for (int y = 2; y < 10; y++)
            for (int x = 6; x < 18; x++)
                p(ox + x, y, C(245, 210, 180));
        // Red cap
        for (int y = 0; y < 3; y++)
            for (int x = 5; x < 19; x++)
                p(ox + x, y, C(200, 40, 40));
        for (int x = 7; x < 17; x++) p(ox + x, 3, C(200, 40, 40));
        // Cap brim
        for (int x = 3; x < 21; x++) p(ox + x, 4, C(180, 30, 30));
        // Eyes
        p(ox + 9, 5, C(0,0,0)); p(ox + 14, 5, C(0,0,0));
        p(ox + 8, 5, C(255,255,255)); p(ox + 13, 5, C(255,255,255));
        // Body (blue overalls)
        for (int y = 11; y < 22; y++)
            for (int x = 5; x < 19; x++)
                p(ox + x, y, C(40, 80, 200));
        // Arms (skin)
        for (int y = 11; y < 18; y++) {
            p(ox + 2, y, C(245,210,180)); p(ox + 3, y, C(245,210,180));
            p(ox + 20, y, C(245,210,180)); p(ox + 21, y, C(245,210,180));
        }
        // Overall straps
        for (int x = 8; x < 11; x++) for (int y = 10; y < 13; y++) p(ox + x, y, C(30, 60, 180));
        for (int x = 13; x < 16; x++) for (int y = 10; y < 13; y++) p(ox + x, y, C(30, 60, 180));
        // Overall buttons
        p(ox + 9, 12, C(255,215,0)); p(ox + 14, 12, C(255,215,0));
        // Legs (different per frame for walk animation)
        int l1 = 4, r1 = 10, l2 = 14, r2 = 20; // default leg positions
        if (frame == 1) { l1 = 6; r1 = 12; l2 = 12; r2 = 18; } // walk spread
        for (int y = 22; y < 30; y++) {
            for (int x = l1; x < r1; x++) p(ox + x, y, C(30, 60, 180));
            for (int x = l2; x < r2; x++) p(ox + x, y, C(30, 60, 180));
        }
        // Shoes (brown)
        for (int y = 28; y < 32; y++) {
            for (int x = l1 - 1; x < r1 + 1; x++) p(ox + x, y, C(100, 60, 30));
            for (int x = l2 - 1; x < r2 + 1; x++) p(ox + x, y, C(100, 60, 30));
        }
    }
    return img;
}

// Barrel: 24x24 with rotation marker visible
static sf::Image make_barrel() {
    sf::Image img;
    img.resize({24, 24}, sf::Color::Transparent);
    auto p = [&](int x, int y, sf::Color c) { sp(img, {x, y}, c); };
    for (int y = 0; y < 24; y++)
        for (int x = 0; x < 24; x++) {
            float dx = x - 12, dy = y - 12;
            if (dx * dx + dy * dy <= 144)
                p(x, y, C(220, 60, 60));
        }
    // Bands
    for (int x = 0; x < 24; x++) {
        for (int y = 6; y < 10; y++) p(x, y, C(160, 40, 40));
        for (int y = 14; y < 18; y++) p(x, y, C(160, 40, 40));
    }
    // Arrow marker to show rotation
    for (int x = 10; x < 14; x++) p(x, 2, C(255, 200, 50));
    p(11, 1, C(255, 200, 50)); p(12, 1, C(255, 200, 50));
    // Highlight
    for (int y = 3; y < 7; y++)
        for (int x = 4; x < 10; x++)
            p(x, y, C(240, 100, 100));
    return img;
}

// DK: 72x84 (bigger, more detailed)
static sf::Image make_donkey_kong() {
    sf::Image img;
    img.resize({72, 84}, sf::Color::Transparent);
    auto p = [&](int x, int y, sf::Color c) { sp(img, {x, y}, c); };

    auto fill = [&](int x1, int y1, int x2, int y2, sf::Color c) {
        for (int y = y1; y < y2; y++) for (int x = x1; x < x2; x++) p(x, y, c);
    };

    // Body (brown)
    fill(14, 24, 58, 66, C(120, 80, 40));
    // Belly (lighter)
    fill(24, 30, 48, 57, C(160, 120, 60));
    // Head
    fill(18, 4, 54, 27, C(130, 90, 50));
    // Ears
    fill(8, 6, 18, 15, C(100, 60, 30));
    fill(54, 6, 64, 15, C(100, 60, 30));
    // Inner ears
    fill(10, 8, 16, 13, C(160, 100, 60));
    fill(56, 8, 62, 13, C(160, 100, 60));
    // Eyes (white)
    fill(24, 15, 30, 19, C(255, 255, 255));
    fill(42, 15, 48, 19, C(255, 255, 255));
    // Pupils
    fill(26, 16, 28, 18, C(0, 0, 0));
    fill(44, 16, 46, 18, C(0, 0, 0));
    // Eyebrows
    fill(22, 12, 32, 14, C(80, 40, 20));
    fill(40, 12, 50, 14, C(80, 40, 20));
    // Nose
    fill(33, 20, 39, 24, C(80, 40, 20));
    // Mouth
    fill(27, 25, 45, 27, C(80, 40, 20));
    // Red tie
    fill(33, 27, 39, 36, C(200, 50, 50));
    fill(35, 36, 37, 40, C(200, 50, 50));
    // Arms
    fill(6, 27, 14, 45, C(110, 70, 35));
    fill(58, 27, 66, 45, C(110, 70, 35));
    // Hands
    fill(4, 39, 14, 45, C(80, 50, 25));
    fill(58, 39, 68, 45, C(80, 50, 25));
    // Legs
    fill(18, 63, 30, 84, C(100, 60, 30));
    fill(42, 63, 54, 84, C(100, 60, 30));
    // Feet
    fill(12, 76, 32, 84, C(80, 40, 20));
    fill(40, 76, 60, 84, C(80, 40, 20));
    // Belly button
    fill(35, 42, 37, 44, C(120, 80, 40));
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

// Background: 800x750 gradient sky
static sf::Image make_background() {
    sf::Image img;
    img.resize({800, 750}, sf::Color::Transparent);
    // Sky gradient: dark blue (top) to lighter blue (bottom)
    for (int y = 0; y < 600; y++) {
        int b = 40 + y / 6;
        int g = 10 + y / 15;
        int r = 5 + y / 20;
        sf::Color c(r > 255 ? 255 : r, g > 120 ? 120 : g, b > 200 ? 200 : b);
        for (int x = 0; x < 800; x++) img.setPixel({(unsigned)x, (unsigned)y}, c);
    }
    // Ground
    for (int y = 600; y < 750; y++) {
        sf::Color gnd(30, 60 + (y - 600) / 3, 15);
        for (int x = 0; x < 800; x++) img.setPixel({(unsigned)x, (unsigned)y}, gnd);
    }
    // Stars at top
    auto star = [&](int sx, int sy) {
        for (int dy = -1; dy <= 1; dy++)
            for (int dx = -1; dx <= 1; dx++)
                if (std::abs(dx) + std::abs(dy) <= 1)
                    img.setPixel({(unsigned)(sx + dx), (unsigned)(sy + dy)}, sf::Color(255, 255, 200));
    };
    star(100, 30); star(250, 80); star(400, 20); star(550, 55);
    star(700, 40); star(150, 110); star(620, 90); star(350, 120);
    // Tall grass on ground
    for (int x = 0; x < 800; x += 8) {
        for (int dy = 0; dy < 4; dy++)
            img.setPixel({(unsigned)(x + 2), (unsigned)(595 + dy)}, sf::Color(25, 120, 10));
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
    return 0;
}
