#ifndef DK_GAME_H
#define DK_GAME_H

#include <SFML/Audio.hpp>
#include <vector>
#include <memory>
#include <string>

#include "model/GameState.hpp"
#include "model/Player.hpp"
#include "model/Platform.hpp"
#include "model/Barrel.hpp"
#include "model/DonkeyKong.hpp"
#include "view/GameView.hpp"
#include "control/GameController.hpp"

/// Top-level coordinator: owns Model objects (Player, platforms, barrels, etc.),
/// a GameState, a GameView for rendering, and a GameController for input.
/// Runs the game loop and implements the core game logic in update().
class Game {
public:
    Game();
    void run();

private:
    void update(float dt);
    void start_game();
    void setup_stage();
    void spawn_barrel();
    void check_collisions();
    void play_random_music();

    GameState state;
    GameView view;
    Player player;
    DonkeyKong dk{60.f, 140.f, 1};
    std::vector<Platform> platforms;
    std::vector<Ladder> ladders;
    std::vector<std::unique_ptr<Barrel>> barrels;
    float barrel_timer = 0;
    float lava_anim = 0;
    sf::Music music;
    GameController controller;

    static constexpr float BARREL_INTERVAL = 2.0f;
};

#endif
