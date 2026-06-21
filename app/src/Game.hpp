#ifndef DK_GAME_H
#define DK_GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Player.hpp"
#include "Platform.hpp"
#include "Barrel.hpp"
#include "DonkeyKong.hpp"

class Game {
public:
    Game();
    void run();

private:
    enum class State { Menu, Playing, GameOver, Won };

    void update(float dt);
    void draw();
    void handle_input(float dt);
    void spawn_barrel();
    void check_collisions();
    void start_game();
    void setup_stage();
    int get_stage() const;
    sf::FloatRect get_segment_rect(int pi, int s) const;
    bool is_solid(int pi, int s) const;

    sf::RenderWindow window;
    sf::Font font;
    sf::Text title_text{font};
    sf::Text status_text{font};
    sf::Text crowns_text{font};
    sf::Text stage_text{font};

    sf::RectangleShape menu_btn;
    sf::Text menu_btn_text{font};

    sf::RectangleShape pause_resume_btn;
    sf::Text pause_resume_text{font};
    sf::RectangleShape pause_reset_btn;
    sf::Text pause_reset_text{font};

    Player player;
    DonkeyKong dk{60.f, 140.f};
    sf::RectangleShape princess;
    sf::Texture princess_tex;

    sf::Texture crown_tex;
    int crowns = 0;
    int stage = 1;

    sf::Texture bg_tex;
    sf::Texture plat_tex;
    sf::Texture ladder_tex;
    sf::Texture lava_tex;
    sf::RectangleShape bg_shape{{800, 750}};

    std::vector<Platform> platforms;
    std::vector<Ladder> ladders;
    std::vector<std::unique_ptr<Barrel>> barrels;
    std::vector<std::vector<bool>> platform_segments;

    float barrel_timer = 0;
    float pause_cd = 0;
    float skip_cd = 0;
    float lava_anim = 0;
    State state = State::Menu;
    bool paused = false;

    static constexpr float BARREL_INTERVAL = 2.0f;
};

#endif
