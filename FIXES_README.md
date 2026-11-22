# Card Game – room/model integration fix

## What was fixed

- `main.cpp` now constructs `Room` and `RoomDecor` after the OpenGL context is initialized.
- The render loop now draws the room and its decorations before the table/cards.
- The bundled `models/Adventurer.glb` is now loaded using its `CharacterArmature|Idle` animation and drawn in the back-right area of the room.
- The room ceiling is hidden in top-down camera mode and enabled in player-view mode.

## Build (Linux)

From this folder:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j2
./build/cardGame
```

If GLFW is not installed, CMake's existing FetchContent fallback needs internet access to download GLFW. On systems with GLFW installed, CMake uses the installed package.

## Controls

- `1`: top-down camera
- `2`: player camera
- `G`: play a random card
- `Esc`: quit

The model and all textures are referenced relative to the project root through `ASSET_DIR`.
