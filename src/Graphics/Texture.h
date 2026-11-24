#pragma once
#include <string>


class Texture {
public:
    explicit Texture(const std::string& path);
    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    void bind() const;  

private:
    unsigned int id_ = 0;
};