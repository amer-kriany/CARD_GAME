#pragma once


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