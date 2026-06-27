#include "GameController.hpp"
#include "../view/GameView.hpp"
#include "../model/GameState.hpp"
#include "../model/Player.hpp"
#include "../model/Platform.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <algorithm>

GameController::GameController(GameView& view, GameState& state, Player& player,
                               std::vector<Ladder>& ladders,
                               sf::Music& music,
                               std::function<void()> on_start_game,
                               std::function<void()> on_play_music,
                               std::function<void()> on_title)
    : view(view), state(state), player(player), ladders(ladders), music(music),
      start_game_fn(std::move(on_start_game)),
      play_music_fn(std::move(on_play_music)),
      title_fn(std::move(on_title)) {}

void GameController::handle_input(float dt) {
    pause_cd -= dt;
    while (auto ev = view.window.pollEvent()) {
        if (ev->is<sf::Event::Closed>()) {
            state.save_records();
            view.window.close();
        }

        if (auto* mb = ev->getIf<sf::Event::MouseButtonPressed>()) {
            sf::Vector2f wp = view.window.mapPixelToCoords(mb->position);

            // Mute always available
            if (view.get_mute_btn_bounds().contains(wp)) {
                state.muted = !state.muted;
                music.setVolume(state.muted ? 0.f : 15.f);
            }

            // ── TITLE SCREEN ──
            if (state.phase == GameState::Phase::Title) {
                if (GameView::title_btn_easy().contains(wp))
                    state.difficulty = GameState::Difficulty::Easy;
                else if (GameView::title_btn_normal().contains(wp))
                    state.difficulty = GameState::Difficulty::Normal;
                else if (GameView::title_btn_hard().contains(wp))
                    state.difficulty = GameState::Difficulty::Hard;
                else if (GameView::title_btn_custom().contains(wp)) {
                    state.difficulty = GameState::Difficulty::Custom;
                    state.phase = GameState::Phase::Custom;
                } else if (GameView::title_btn_play().contains(wp)) {
                    state.overall_timer = 0;
                    state.lives = 3;
                    state.coins = 0;
                    state.last_pickup_stage = 0;
                    title_fn();
                    start_game_fn();
                }
            }

            // ── CUSTOM DIFFICULTY SCREEN ──
            if (state.phase == GameState::Phase::Custom) {
                auto clamp = [](float v, float lo, float hi) { return std::max(lo, std::min(hi, v)); };

                if (GameView::custom_speed_track().contains(wp)) {
                    float t = (wp.x - 250.f) / 300.f;
                    state.custom_speed = clamp(60.f + t * (400.f - 60.f), 60.f, 400.f);
                } else if (GameView::custom_interval_track().contains(wp)) {
                    float t = (wp.x - 250.f) / 300.f;
                    state.custom_interval = clamp(0.5f + t * (5.f - 0.5f), 0.5f, 5.f);
                } else if (GameView::custom_btn_play().contains(wp)) {
                    state.overall_timer = 0;
                    state.lives = 3;
                    title_fn();
                    start_game_fn();
                }
            }

            // ── PAUSED ──
            if (state.phase == GameState::Phase::Playing && state.paused) {
                if (view.get_pause_resume_btn_bounds().contains(wp)) {
                    state.paused = false;
                }
                if (view.get_pause_reset_btn_bounds().contains(wp)) {
                    state.paused = false;
                    state.overall_timer = 0;
                    state.lives = 3;
                    title_fn();
                }
            }

            // ── GAME OVER ──
            if (state.phase == GameState::Phase::GameOver) {
                if (view.get_menu_btn_bounds().contains(wp)) {
                    state.overall_timer = 0;
                    state.lives = 3;
                    play_music_fn();
                    title_fn();
                }
            }

            // ── STAGE COMPLETE / WON ──
            if (state.phase == GameState::Phase::Won) {
                if (state.crowns >= 9) {
                    // Play Again = menu_btn
                    if (view.get_menu_btn_bounds().contains(wp)) {
                        state.overall_timer = 0;
                        state.lives = 3;
                        title_fn();
                        start_game_fn();
                    }
                } else {
                    // Next Stage = menu_btn (keeps overall_timer)
                    if (view.get_menu_btn_bounds().contains(wp)) {
                        state.crowns++;
                        state.lives = 3;
                        start_game_fn();
                    }
                }
                // MENU = pause_reset_btn
                if (view.get_pause_reset_btn_bounds().contains(wp)) {
                    state.overall_timer = 0;
                    state.lives = 3;
                    title_fn();
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

        bool jump_down = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) ||
                         sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
                         sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);
        if (jump_down && !jump_key_held)
            player.jump();
        jump_key_held = jump_down;
    }
}
