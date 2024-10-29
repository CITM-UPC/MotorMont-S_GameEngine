#include "Texture.h"
#include <iostream>
#include <string>

Texture::~Texture() {
    if (textureID) {
        glDeleteTextures(1, &textureID);
    }
}

bool Texture::loadFromFile(const std::string& filePath) {
#ifdef IL_VERSION 
    ilInit();
    ILuint imageID;
    ilGenImages(1, &imageID);
    ilBindImage(imageID);

    if (!ilLoadImage((const wchar_t*)filePath.c_str())) {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
        ilDeleteImages(1, &imageID);
        return false;
    }

    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA,
        ilGetInteger(IL_IMAGE_WIDTH),
        ilGetInteger(IL_IMAGE_HEIGHT),
        0, GL_RGBA, GL_UNSIGNED_BYTE,
        ilGetData()
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GLWrapMode(wrapMode));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GLWrapMode(wrapMode));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GLMinFilter(filter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GLMagFilter(filter));

    ilDeleteImages(1, &imageID);
    return true;
#else
    std::cerr << "DevIL library not found. Ensure DevIL is correctly installed and linked." << std::endl;
    return false;
#endif
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, textureID);
}
