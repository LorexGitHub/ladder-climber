# Donkey Kong

A C++ Donkey Kong remake with 9 stages, 8 unique bosses, procedural level generation, and a music system. Built with SFML 3.0 following the Model-View-Controller pattern.

## Gameplay

- Climb 9 stages by navigating platforms and ladders while dodging barrels
- Each stage has a unique boss sprite
- Collect a crown at the top of each stage
- Stage 1: 2 platforms, Stage 2: 4 platforms, Stage 3+: 6 platforms
- Holes appear in platforms from Stage 4 onward
- 3 lives per run — die on a stage to retry; lose all lives for game over
- Stage complete screen shows time vs record — choose NEXT STAGE or RESET (retry)
- After 9 crowns, the overall time is compared to your best — choose PLAY AGAIN

## Controls

| Key | Action |
|-----|--------|
| ← → / A D | Move left/right |
| Space / W / ↑ | Jump |
| ↑ / W | Climb up ladder |
| ↓ / S | Climb down ladder |
| Esc / P | Pause |
| M | Toggle mute |
| U / I | Skip to previous / next stage (debug) |

## Architecture (MVC)

**Model** (`src/model/`)
- `Player` — position, velocity, jump/climb state, animation
- `Platform` — segmented platforms with destructible holes; `Ladder` struct
- `Barrel` — rolling obstacle with platform collision
- `DonkeyKong` — boss character with per-stage sprite
- `GameState` — stage, crowns, lives, timers, records, phase enum

**View** (`src/view/`)
- `GameView` — owns the `sf::RenderWindow`, fonts, textures, and all `draw()` logic

**Control** (`src/control/`)
- `GameController` — processes keyboard/mouse input, modifies model via callbacks

**Coordinator** (`src/Game.hpp/.cpp`)
- `Game` — owns Model objects, View, Controller; runs the game loop and core update logic

## Building

### Prerequisites
- CMake ≥ 3.22
- C++20 compiler (GCC, Clang, or MSVC)
- SFML 3.0 + GoogleTest (downloaded automatically by CMake)

### System dependencies (Linux/WSL)
```bash
sudo apt install build-essential cmake \
  libx11-dev libxrandr-dev libxcursor-dev libxi-dev \
  libgl1-mesa-dev libglu1-mesa-dev \
  libudev-dev libfreetype-dev libvorbis-dev libogg-dev libflac-dev
```

### Build & run
```bash
cd app
mkdir build && cd build
cmake ..
cmake --build . -j4
LD_LIBRARY_PATH=bin ./bin/DonkeyKong
```

### Tests
```bash
cd app/build
cmake --build . -j4
LD_LIBRARY_PATH=bin ./bin/DonkeyKong_test
```

## File structure
```
donkey-kong/
├── app/
│   ├── CMakeLists.txt
│   ├── src/
│   │   ├── main.cpp
│   │   ├── Game.hpp / Game.cpp
│   │   ├── model/
│   │   │   ├── GameState.hpp / GameState.cpp
│   │   │   ├── Player.hpp / Player.cpp
│   │   │   ├── Platform.hpp / Platform.cpp
│   │   │   ├── Barrel.hpp / Barrel.cpp
│   │   │   └── DonkeyKong.hpp / DonkeyKong.cpp
│   │   ├── view/
│   │   │   └── GameView.hpp / GameView.cpp
│   │   ├── control/
│   │   │   └── GameController.hpp / GameController.cpp
│   │   └── tools/
│   │       └── gen_sprites.cpp
│   ├── test/
│   │   ├── GameStateTest.cpp
│   │   ├── PlayerTest.cpp
│   │   └── BarrelTest.cpp
│   └── assets/
│       ├── sprites/    # generated .png files
│       ├── music/      # .mp3 files
│       └── fonts/      # arial.ttf (copied from system)
└── README.md
```

## Dependencies

All libraries pulled automatically by CMake via FetchContent:
- [SFML 3.0](https://github.com/SFML/SFML) — windowing, graphics, audio, input
- [GoogleTest](https://github.com/google/googletest) — unit tests
