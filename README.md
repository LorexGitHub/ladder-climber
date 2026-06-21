# Donkey Kong

A C++ Donkey Kong remake with 9 stages, 8 unique bosses, procedural level generation, and a music system.

## Gameplay

- Climb 9 stages by navigating platforms and ladders while avoiding barrels
- Each stage has a unique boss sprite (not just tinted DK)
- Collect a crown at the top of each stage
- Stage 1: 2 platforms, Stage 2: 4 platforms, Stage 3+: 6 platforms
- Holes appear in platforms from Stage 4 onward
- After finishing a stage, the stage complete screen shows your time, previous record, and overall timer — choose **NEXT STAGE** or **RESET** (retry the same stage)
- After collecting all 9 crowns, the overall time is compared to your best — choose **PLAY AGAIN**

## Controls

| Key | Action |
|-----|--------|
| ← → | Move left/right |
| Space | Jump (hold while falling for bunny hop) |
| M | Toggle mute |
| U / I | Skip to previous / next stage (debug) |

## Features

- **Air acceleration** — player can steer mid-air for skill-based movement
- **Per-stage color tint** — each stage has a unique color palette
- **Animated torches** — flickering flames on towers and platforms
- **City silhouette** — rendered in the background
- **Records** — per-stage and overall best times saved to `records.txt`
- **Music** — random `.flac` files from `assets/music/`; new song on death or PLAY AGAIN
- **Stage skip** — press U/I to jump stages

## Building

### Prerequisites
- CMake ≥ 3.22
- C++20 compiler (GCC, Clang, or MSVC)
- SFML 3.0 (downloaded automatically by CMake)

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

## File structure
```
donkey-kong/
├── app/
│   ├── CMakeLists.txt
│   ├── src/
│   │   ├── main.cpp
│   │   ├── Game.hpp / Game.cpp
│   │   ├── Player.hpp / Player.cpp
│   │   ├── Barrel.hpp / Barrel.cpp
│   │   ├── DonkeyKong.hpp / DonkeyKong.cpp
│   │   ├── Platform.hpp / Platform.cpp
│   │   └── gen_sprites.cpp          # procedural sprite generation
│   └── assets/
│       ├── sprites/                  # generated .png files
│       └── music/                    # .flac files
└── README.md
```

## Dependencies

All libraries are pulled automatically by CMake via FetchContent:
- [SFML 3.0](https://github.com/SFML/SFML) — windowing, graphics, audio, input
