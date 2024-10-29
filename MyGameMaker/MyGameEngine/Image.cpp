#include "Image.h"
#include <vector>
#include <GL/glew.h>
#include <IL/il.h>
#include <IL/ilu.h>

Image::~Image() {
    if (_id) glDeleteTextures(1, &_id);
}

Image::Image(Image&& other) noexcept :
    _id(other._id),
    _width(other._width),
    _height(other._height),
    _channels(other._channels) {
    other._id = 0;
}

void Image::bind() const {
    glBindTexture(GL_TEXTURE_2D, _id);
}

static auto formatFromChannels(int channels) {
    switch (channels) {
    case 1: return GL_LUMINANCE;
    case 2: return GL_LUMINANCE_ALPHA;
    case 3: return GL_RGB;
    case 4: return GL_RGBA;
    default: return GL_RGB;
    }
}

static int rowAlignment(int width, int channels) {
    const size_t rowSizeInBytes = static_cast<size_t>(width) * channels;
    if ((rowSizeInBytes % 8) == 0) return 8;
    if ((rowSizeInBytes % 4) == 0) return 4;
    if ((rowSizeInBytes % 2) == 0) return 2;
    return 1;
}

void Image::load(int width, int height, int channels, void* data) {
    _width = width;
    _height = height;
    _channels = channels;

    if (!_id) glGenTextures(1, &_id);

    bind();
    glPixelStorei(GL_UNPACK_ALIGNMENT, rowAlignment(width, channels));
    glTexImage2D(GL_TEXTURE_2D, 0, channels, width, height, 0, formatFromChannels(channels), GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

bool Image::loadFromFile(const std::string& path) {
    ILuint imageID;
    ilGenImages(1, &imageID);
    ilBindImage(imageID);

    if (!ilLoadImage((const wchar_t*)path.c_str())) {
        ilDeleteImages(1, &imageID);
        return false;
    }

    _width = ilGetInteger(IL_IMAGE_WIDTH);
    _height = ilGetInteger(IL_IMAGE_HEIGHT);
    _channels = ilGetInteger(IL_IMAGE_CHANNELS);

    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

    if (!_id) glGenTextures(1, &_id);
    bind();

    glTexImage2D(GL_TEXTURE_2D, 0, _channels, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());

    ilDeleteImages(1, &imageID);
    return true;
}
