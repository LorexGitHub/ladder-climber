#ifndef DK_GAMEVIEW_H
#define DK_GAMEVIEW_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>

class Player;
class Platform;
struct Ladder;
class Barrel;
class DonkeyKong;
class Coin;
class PowerUp;
class GameState;

class GameView {
public:
    GameView();

    void draw(const GameState& state, const Player& player,
              const std::vector<Platform>& platforms,
              const std::vector<Ladder>& ladders,
              const std::vector<std::unique_ptr<Barrel>>& barrels,
              const DonkeyKong& dk, float lava_anim,
              const std::vector<Coin>& coins,
              const PowerUp* powerup);

    sf::FloatRect get_menu_btn_bounds() const;
    sf::FloatRect get_pause_resume_btn_bounds() const;
    sf::FloatRect get_pause_reset_btn_bounds() const;
    sf::FloatRect get_mute_btn_bounds() const;

    static sf::FloatRect title_btn_easy();
    static sf::FloatRect title_btn_normal();
    static sf::FloatRect title_btn_hard();
    static sf::FloatRect title_btn_custom();
    static sf::FloatRect title_btn_play();

    static sf::FloatRect custom_speed_track();
    static sf::FloatRect custom_interval_track();
    static sf::FloatRect custom_btn_play();

    sf::RenderWindow window;

private:
    sf::Font font;

    sf::Text title_text{font};
    sf::Text status_text{font};
    sf::Text crowns_text{font};
    sf::Text stage_text{font};
    sf::Text time_text{font};
    sf::Text record_text{font};
    sf::Text overall_rec_text{font};
    sf::Text stage_rec_text{font};

    sf::RectangleShape menu_btn;
    sf::Text menu_btn_text{font};
    sf::RectangleShape pause_resume_btn;
    sf::Text pause_resume_text{font};
    sf::RectangleShape pause_reset_btn;
    sf::Text pause_reset_text{font};

    sf::RectangleShape mute_btn;
    sf::Text mute_text{font};

    sf::RectangleShape princess;
    sf::Texture princess_tex;

    sf::Texture crown_tex;
    sf::Texture bg_tex;
    sf::Texture plat_tex;
    sf::Texture ladder_tex;
    sf::Texture lava_tex;
    sf::RectangleShape bg_shape{{800, 750}};

    sf::FloatRect draw_btn(sf::FloatRect rect, const std::string& label, const sf::Color& color);
    float draw_slider(float cx, float cy, float min_val, float max_val, float val);

    bool load_font();
};

#endif
