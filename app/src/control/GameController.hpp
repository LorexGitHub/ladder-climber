#ifndef DK_GAMECONTROLLER_H
#define DK_GAMECONTROLLER_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <functional>
#include <vector>

class GameView;
class GameState;
class Player;
struct Ladder;

/// Processes user input (keyboard and mouse) and translates it into
/// model state changes and game commands via callbacks.
class GameController {
public:
    GameController(GameView& view, GameState& state, Player& player,
                   std::vector<Ladder>& ladders,
                   sf::Music& music,
                   std::function<void()> on_start_game,
                   std::function<void()> on_play_music);

    /// Polls events and reads keyboard state, then modifies model accordingly.
    void handle_input(float dt);

private:
    GameView& view;
    GameState& state;
    Player& player;
    std::vector<Ladder>& ladders;
    sf::Music& music;
    std::function<void()> start_game_fn;
    std::function<void()> play_music_fn;
    float pause_cd = 0;
};

#endif
