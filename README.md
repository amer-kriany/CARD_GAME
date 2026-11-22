# Royal Card Game 🃏

A small 3D card game scene made for the **Computer Graphics** course.
C++17 · OpenGL 3.3 (core) · GLFW · GLAD · GLM · CMake

A player sits at a wooden table in a room. Press **G** to play random cards
from your deck onto the central field. Two fixed cameras show the scene from
above (cards lie flat) or from the player's seat (cards stand up).

## Team

| Member | Part |
|---|---|
| **Amer** | Table, card management, card positions, integration |
| **Wassim** | Card art / room (floor, walls, poster) |
| **Fadi** | Cameras and room decoration |

## Controls

| Key | Action |
|---|---|
| `G` | Move a random card from the deck to the field (after 5 cards, nothing happens) |
| `1` | Camera 1: top-down view, cards lie flat |
| `2` | Camera 2: player view, cards stand up |
| `Esc` | Quit |

## What is in the scene

- **Table** with 3 zones: player deck, central field, enemy field (wood top, gold/leather details), plus apron and 4 legs.
- **5 cards**, each with its own texture (`textures/cards/`). Played cards fly in an arc to the field.
- **Room**: wooden floor, wallpaper-style walls, ceiling (camera 2 only) and a poster with a torn corner.
- **3 decoration pieces**: bookshelf, spinning Saturn, and a 3D character model (the only 3D model).

## Build and run

CMake downloads and builds GLFW automatically if it is not installed.
Asset paths are baked in at build time (`ASSET_DIR`), so the game runs from any folder.

### Ubuntu / Linux

```bash
sudo apt install build-essential cmake git libglfw3-dev libgl1-mesa-dev
git clone <repo-url> cardGame
cd cardGame
cmake -S . -B build
cmake --build build -j
./build/cardGame
```

### Windows (Visual Studio)

1. Install **Visual Studio 2022** (Community) with the workload *Desktop development with C++* (it includes CMake), and **Git**.
2. Open *x64 Native Tools Command Prompt for VS 2022* (or PowerShell) and run:

```bat
git clone <repo-url> cardGame
cd cardGame
cmake -S . -B build
cmake --build build --config Release
build\Release\cardGame.exe
```

GLFW is downloaded automatically on the first configure (needs internet).
Alternative: open the `cardGame` folder in Visual Studio or VS Code (with the CMake Tools extension), select **Release**, then build and run.

### Windows (MinGW / MSYS2)

```bash
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
build/cardGame.exe
```

### Troubleshooting

- **Black window or crash on start:** update your graphics driver. OpenGL 3.3 is required.
- **Textures or shaders not found:** do not move the `textures/`, `shaders/` and `models/` folders. Re-run CMake if you moved the project.

## Project structure

```
cardGame/
├── CMakeLists.txt
├── include/            glad, KHR, stb_image.h, cgltf.h
├── models/             Adventurer.glb (3D character)
├── shaders/            basic.vert, basic.frag
├── textures/           wood, room textures, cards/ (card faces)
└── src/
    ├── main.cpp        window, main loop, drawing order
    ├── WorldConfig.h   shared scene numbers (scales, floor height)
    ├── Cards/          Card, CardManager (logic), CardAnimator (movement), Cards (card meshes)
    ├── Table/          Table, TableZone, TableLegs
    ├── Camera/         DualCamera (keys 1 / 2)
    ├── Room/           Room (floor, walls, poster), RoomDecor (bookshelf, Saturn)
    └── Graphics/       Shader, Texture, Quad, Cube, Polygon, Model (glTF loader)
```

## How it works (short)

- **Logic and visuals are separate.** `CardManager` decides which card moves; `CardAnimator` only draws where it is. They share the card id.
- **Matrix chain** in the vertex shader: `projection * view * world * model`.
- **Materials** are chosen in the fragment shader with the `useTexture` value (textured, wood, gold, leather, room, lit solid colour).
- **Cameras** are fixed; switching modes also stands the cards up or lays them flat.

## Credits

- Character model: *Adventurer* by Quaternius (CC0).
- Libraries: GLFW, GLAD, GLM, stb_image, cgltf.
- Card characters: *The Amazing World of Gumball* (Cartoon Network), used as placeholder art for a university project. Replace with team-based characters if required.
- Textures: team-provided / free textures.

## AI usage

All prompts used during development are listed in `prompts.txt`.