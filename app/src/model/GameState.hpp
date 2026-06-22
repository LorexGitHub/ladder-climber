#ifndef DK_GAMESTATE_H
#define DK_GAMESTATE_H

#include <string>
#include <cstdio>

/// Holds per-stage and overall best times, persisted to disk.
struct TimeRecord {
    float stage_times[9] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};
    float overall_time = -1;
};

/// Pure game state: phase, stage progress, timers, lives, and records.
/// Owned by Game and read/written by GameController and GameView.
class GameState {
public:
    enum class Phase { Menu, Playing, GameOver, Won };

    int stage = 1;
    int crowns = 0;
    int lives = 3;
    bool paused = false;
    Phase phase = Phase::Menu;
    float overall_timer = 0.f;
    float stage_timer = 0.f;
    float skip_cd = 0.f;
    bool muted = false;

    TimeRecord records;

    /// Persists records to disk.
    void save_records(const char* path = "assets/times.dat");
    /// Loads records from disk.
    void load_records(const char* path = "assets/times.dat");
    /// Formats a time in seconds as "MM:SS.hh" (or "NONE" if negative).
    static std::string fmt_time(float t);
};

#endif
