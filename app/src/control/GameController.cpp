#include "GameController.hpp"
#include "../view/GameView.hpp"
#include "../model/GameState.hpp"
#include "../model/Player.hpp"
#include "../model/Platform.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

GameController::GameController(GameView& view, GameState& state, Player& player,
                               std::vector<Ladder>& ladders,
                               sf::Music& music,
                               std::function<void()> on_start_game,
                               std::function<void()> on_play_music)
    : view(view), state(state), player(player), ladders(ladders), music(music),
      start_game_fn(std::move(on_start_game)),
      play_music_fn(std::move(on_play_music)) {}

void GameController::handle_input(float dt) {
    pause_cd -= dt;
    while (auto ev = view.window.pollEvent()) {
        if (ev->is<sf::Event::Closed>()) {
            state.save_records();
            view.window.close();
        }

        if (auto* mb = ev->getIf<sf::Event::MouseButtonPressed>()) {
            sf::Vector2f wp = view.window.mapPixelToCoords(mb->position);

            if (view.get_mute_btn_bounds().contains(wp)) {
                state.muted = !state.muted;
                music.setVolume(state.muted ? 0.f : 15.f);
            }

            if (state.phase == GameState::Phase::Playing && state.paused) {
                if (view.get_pause_resume_btn_bounds().contains(wp))
                    state.paused = false;
                if (view.get_pause_reset_btn_bounds().contains(wp)) {
                    state.overall_timer = 0;
                    state.lives = 3;
                    start_game_fn();
                }
            }

            if (state.phase == GameState::Phase::Menu ||
                state.phase == GameState::Phase::GameOver ||
                state.phase == GameState::Phase::Won) {
                if (state.phase == GameState::Phase::Won &&
                    view.get_pause_reset_btn_bounds().contains(wp)) {
                    start_game_fn();
                } else if (view.get_menu_btn_bounds().contains(wp)) {
                    if (state.phase == GameState::Phase::GameOver ||
                        (state.phase == GameState::Phase::Won && state.crowns >= 9)) {
                        state.overall_timer = 0;
                        state.lives = 3;
                        play_music_fn();
                    }
                    start_game_fn();
                }
            }
        }
    }

    // Stage skip keys (U = back, I = forward)
    state.skip_cd -= dt;
    if (state.skip_cd <= 0) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::U)) {
            if (state.crowns > 0) state.crowns--;
            start_game_fn();
            state.skip_cd = 0.3f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::I)) {
            if (state.crowns < 8) state.crowns++;
            start_game_fn();
            state.skip_cd = 0.3f;
        }
    }

    if (state.phase == GameState::Phase::Playing) {
        if (pause_cd <= 0 &&
            (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape) ||
             sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P))) {
            state.paused = !state.paused;
            pause_cd = 1.f;
        }

        // Player movement and climbing input
        bool on_ladder = false;
        for (auto& l : ladders) {
            if (player.get_bounds().findIntersection(l.get_bounds()).has_value()) {
                on_ladder = true;
                break;
            }
        }

        if (on_ladder) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
                player.climb(-180.f);
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
                player.climb(180.f);
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
                player.move_left();
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
                player.move_right();
            else
                player.stop_on_ladder();
        } else {
            if (player.is_climbing())
                player.set_climbing(false);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
                player.move_left();
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
                player.move_right();
            else
                player.stop_horizontal();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
            player.jump();
    }
}
