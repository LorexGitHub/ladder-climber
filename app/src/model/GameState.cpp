#include "GameState.hpp"
#include <fstream>

std::string GameState::fmt_time(float t) {
    if (t < 0) return "NONE";
    int mins = int(t) / 60;
    int secs = int(t) % 60;
    int hs = int((t - int(t)) * 100);
    char buf[16];
    std::snprintf(buf, sizeof(buf), "%02d:%02d.%02d", mins, secs, hs);
    return buf;
}

void GameState::load_records(const char* path) {
    std::ifstream f(path);
    if (!f.is_open()) return;
    for (int i = 0; i < 9; i++) f >> records.stage_times[i];
    f >> records.overall_time;
}

void GameState::save_records(const char* path) {
    std::ofstream f(path);
    if (!f.is_open()) return;
    for (int i = 0; i < 9; i++) f << records.stage_times[i] << "\n";
    f << records.overall_time;
}

float GameState::get_barrel_speed() const {
    switch (difficulty) {
        case Difficulty::Easy:   return 120.f;
        case Difficulty::Normal: return 180.f;
        case Difficulty::Hard:   return 260.f;
        case Difficulty::Custom: return custom_speed;
    }
    return 180.f;
}

float GameState::get_barrel_interval() const {
    switch (difficulty) {
        case Difficulty::Easy:   return 3.0f;
        case Difficulty::Normal: return 2.0f;
        case Difficulty::Hard:   return 1.2f;
        case Difficulty::Custom: return custom_interval;
    }
    return 2.0f;
}
