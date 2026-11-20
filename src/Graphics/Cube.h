
#pragma once

class Cube {
public:
    Cube();
    ~Cube();

    Cube(const Cube&) = delete;
    Cube& operator=(const Cube&) = delete;

    void draw() const;

private:
    unsigned int vao_ = 0;
    unsigned int vbo_ = 0;
};
