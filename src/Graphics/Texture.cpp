#include "Texture.h"
#include <glad/glad.h>
#include <iostream>

// This define must appear in exactly ONE .cpp file in the whole project.
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(const std::string& path) {
    stbi_set_flip_vertically_on_load(true);  // OpenGL's (0,0) is the bottom-left
    int width, height, channels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_2D, id_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data) {
        GLenum format = GL_RGB;
        if (channels == 4) format = GL_RGBA;
        else if (channels == 1) format = GL_RED;

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  // rows are tightly packed
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture: " << path << " ("
                  << stbi_failure_reason() << ")\n";
    }
    stbi_image_free(data);  // the GPU has its own copy now
}

Texture::~Texture() {
    glDeleteTextures(1, &id_);
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, id_);
}