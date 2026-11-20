#pragma once

// One reusable rectangle: 4 corners, each with position + UV.
// It is a "unit" quad (from -0.5 to +0.5), so every table, zone and card
// is this same shape, moved and resized with a model matrix.
class Quad {
public:
    Quad();
    ~Quad();

    Quad(const Quad&) = delete;
    Quad& operator=(const Quad&) = delete;

    void draw() const;

private:
    unsigned int vao_ = 0;  // how to read the data
    unsigned int vbo_ = 0;  // the data itself
};