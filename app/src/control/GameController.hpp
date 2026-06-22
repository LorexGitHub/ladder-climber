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

class GameController {
public:
    GameController(GameView& view, GameState& state, Player& player,
                   std::vector<Ladder>& ladders,
                   sf::Music& music,
                   std::function<void()> on_start_game,
                   std::function<void()> on_play_music,
                   std::function<void()> on_title);

    void handle_input(float dt);

private:
    GameView& view;
    GameState& state;
    Player& player;
    std::vector<Ladder>& ladders;
    sf::Music& music;
    std::function<void()> start_game_fn;
    std::function<void()> play_music_fn;
    std::function<void()> title_fn;
    float pause_cd = 0;
    bool jump_key_held = false;
};

#endif
