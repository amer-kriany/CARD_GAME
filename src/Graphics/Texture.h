#pragma once
#include <string>

// Owns one image on the GPU. Replaces the stbi_load / glGenTextures /
// glTexImage2D code from main().
class Texture {
public:
    explicit Texture(const std::string& path);
    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    void bind() const;  // "use this image for the next draw call"

private:
    unsigned int id_ = 0;
};