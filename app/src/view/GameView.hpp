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
class GameState;

/// Handles all rendering for the game: background, entities, HUD, and overlays.
/// Owns the window, fonts, textures, and all drawable UI elements.
class GameView {
public:
    GameView();

    /// Draws a full frame of the current game state.
    void draw(const GameState& state, const Player& player,
              const std::vector<Platform>& platforms,
              const std::vector<Ladder>& ladders,
              const std::vector<std::unique_ptr<Barrel>>& barrels,
              const DonkeyKong& dk, float lava_anim);

    /// Exposes button bounds for controller hit-testing.
    sf::FloatRect get_menu_btn_bounds() const;
    sf::FloatRect get_pause_resume_btn_bounds() const;
    sf::FloatRect get_pause_reset_btn_bounds() const;
    sf::FloatRect get_mute_btn_bounds() const;

    /// The application window, created by GameView but accessible externally.
    sf::RenderWindow window;

private:
    sf::Font font;

    // Text objects
    sf::Text title_text{font};
    sf::Text status_text{font};
    sf::Text crowns_text{font};
    sf::Text stage_text{font};
    sf::Text time_text{font};
    sf::Text record_text{font};
    sf::Text overall_rec_text{font};
    sf::Text stage_rec_text{font};

    // Button backgrounds
    sf::RectangleShape menu_btn;
    sf::Text menu_btn_text{font};
    sf::RectangleShape pause_resume_btn;
    sf::Text pause_resume_text{font};
    sf::RectangleShape pause_reset_btn;
    sf::Text pause_reset_text{font};

    // Mute toggle
    sf::RectangleShape mute_btn;
    sf::Text mute_text{font};

    // Princess
    sf::RectangleShape princess;
    sf::Texture princess_tex;

    // Textures
    sf::Texture crown_tex;
    sf::Texture bg_tex;
    sf::Texture plat_tex;
    sf::Texture ladder_tex;
    sf::Texture lava_tex;
    sf::RectangleShape bg_shape{{800, 750}};

    /// Loads a font from several fallback paths.
    bool load_font();
};

#endif
