# Donkey Kong

## What it is
A C++ implementation of the classic 1980s arcade game Donkey Kong, built with SFML 3.0 and following the Model-View-Controller (MVC) pattern. The player controls a character who must climb platforms, dodge rolling barrels, and reach the top of the level.

## How it works
The game runs a fixed-timestep game loop (60 FPS) inside `Game::start()`. Each iteration:
1. **Input** polls SFML keyboard events and translates them into movement commands
2. **Update** advances player physics (horizontal speed + gravity), barrel physics (rolling + falling), and collision detection (player ↔ platforms)
3. **Draw** clears the window, renders all objects through a compositing `Layer`, and presents the frame

### Architecture

**Model** (`src/model/`)
- **Player** – position, movement direction, jump state. Physics uses a parabolic arc for jumping and constant gravity for falling.
- **Platform** – a static brown rectangle the player stands on.
- **Girder** – a sloped grey rectangle that barrels roll downhill along.
- **Barrel** – a red circle with a two-state machine: *OnGirder* (glued to surface, constant speed) and *Falling* (gravity acceleration).

**View** (`src/view/`)
- **Layer** – an off-screen render buffer that collects drawable objects and composites them onto the window. Supports independent views per layer (e.g. parallax scrolling).

**Controller** (`src/control/`)
- **Game** – top-level controller that owns the window, level geometry, player and barrel controllers. Runs the input/update/draw loop.
- **PlayerControl** – bridges keyboard commands to the Player model; handles platform collision snap and horizontal boundary clamping.
- **BarrelControl** – spawns a barrel on the first girder, advances its physics each frame, and respawns it when it rolls off-screen.

## Building

### Prerequisites
- CMake ≥ 3.22
- C++20 compiler (GCC, Clang, or MSVC)
- SFML 3.0 (downloaded automatically)

### Build & run
```bash
cd DonkeyKong
mkdir build && cd build
cmake ..
cmake --build .
./bin/DonkeyKong
```

### Tests
```bash
cmake --build .
./bin/Test
```

## Controls
| Key | Action |
|-----|--------|
| ← → | Move left/right |
| Space | Jump |

## Dependencies
All libraries are pulled automatically by CMake via FetchContent:
- [SFML 3.0](https://github.com/SFML/SFML) – windowing, graphics, input
- [GoogleTest](https://github.com/google/googletest) – unit tests (test build only)

For system-level dependencies (Linux/WSL):
```bash
sudo apt install build-essential cmake \
  libx11-dev libxrandr-dev libxcursor-dev libxi-dev \
  libgl1-mesa-dev libglu1-mesa-dev \
  libudev-dev libfreetype-dev libvorbis-dev libogg-dev libflac-dev
```

## File structure
```
DonkeyKong/
├── CMakeLists.txt
├── README.md
├── assets/
│   └── images/
│       └── platformer.png
├── src/
│   ├── main.cpp
│   ├── model/
│   │   ├── Player.hpp / .cpp
│   │   ├── Platform.hpp / .cpp
│   │   ├── Girder.hpp / .cpp
│   │   └── Barrel.hpp / .cpp
│   ├── view/
│   │   └── Layer.hpp / .cpp
│   └── control/
│       ├── Game.hpp / .cpp
│       ├── PlayerControl.hpp / .cpp
│       └── BarrelControl.hpp / .cpp
└── test/
    ├── CMakeLists.txt
    ├── GameTest.cpp
    ├── PlayerTest.cpp
    ├── PlatformTest.cpp
    ├── GirderTest.cpp
    ├── BarrelTest.cpp
    ├── PlayerControlTest.cpp
    └── LayerTest.cpp
```