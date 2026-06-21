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
    void load_crowns();
    void save_crowns();

    sf::RenderWindow window;
    sf::Font font;
    sf::Text title_text{font};
    sf::Text status_text{font};
    sf::Text crowns_text{font};

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

    sf::Texture bg_tex;
    sf::Texture plat_tex;
    sf::Texture ladder_tex;
    sf::RectangleShape bg_shape{{800, 750}};

    std::vector<Platform> platforms;
    std::vector<Ladder> ladders;
    std::vector<std::unique_ptr<Barrel>> barrels;

    float barrel_timer = 0;
    float pause_cd = 0;
    State state = State::Menu;
    bool paused = false;

    static constexpr float BARREL_INTERVAL = 2.0f;
};

#endif