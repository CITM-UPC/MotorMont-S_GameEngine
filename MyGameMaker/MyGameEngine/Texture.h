#pragma once

#include <memory>
#include <string>
#include <GL/glew.h>
#include "Image.h"
#include <IL/il.h>

class Texture {
public:
    enum WrapModes { Repeat, MirroredRepeat, Clamp };
    WrapModes wrapMode = Repeat;

    enum Filters { Nearest, Linear };
    Filters filter = Nearest;

private:
    GLuint textureID;
    std::shared_ptr<Image> _img_ptr;

    // Helper functions for wrapping and filtering
    static GLenum GLWrapMode(WrapModes mode) {
        switch (mode) {
        case Repeat: return GL_REPEAT;
        case MirroredRepeat: return GL_MIRRORED_REPEAT;
        case Clamp: return GL_CLAMP_TO_EDGE;
        default: return GL_REPEAT;
        }
    }

    static GLenum GLMagFilter(Filters filter) {
        switch (filter) {
        case Nearest: return GL_NEAREST;
        case Linear: return GL_LINEAR;
        default: return GL_NEAREST;
        }
    }

    static GLenum GLMinFilter(Filters filter) {
        switch (filter) {
        case Nearest: return GL_NEAREST_MIPMAP_NEAREST;
        case Linear: return GL_LINEAR_MIPMAP_LINEAR;
        default: return GL_NEAREST_MIPMAP_LINEAR;
        }
    }

public:
    Texture() : textureID(0) {}
    ~Texture();

    bool loadFromFile(const std::string& filePath);
    void bind() const;
    unsigned int id() const { return textureID; }
    void setImage(const std::shared_ptr<Image>& img_ptr) { _img_ptr = img_ptr; }
    const auto& image() const { return *_img_ptr; }
};
