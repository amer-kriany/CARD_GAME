cardGame/
├── CMakeLists.txt        how to build (finds every src/*.cpp by itself)
├── include/              glad, KHR, stb_image.h   (you already have these)
├── shaders/
│   ├── basic.vert        vertex shader:  model * position
│   └── basic.frag        fragment shader: texture * tint, or a flat color
├── textures/
│   └── wood.png          the table texture
└── src/
    ├── main.cpp          window + game loop only
    ├── glad.c            (you already have it)
    ├── Shader.h/.cpp     compiles and uses a shader program
    ├── Texture.h/.cpp    loads an image onto the GPU
    ├── Quad.h/.cpp       the one reusable rectangle (VAO + VBO)
    └── Table.h/.cpp      the table + the 3 zones (deck, game, enemy)

BUILD AND RUN (from ~/cardGame)
    mkdir -p build && cd build
    cmake ..
    make
    ./cardGame

Because CMake passes the project path into the code (ASSET_DIR), you can run
./cardGame from any folder, and VS Code F5 works without a cwd setting.
Add "build/" to .gitignore.
