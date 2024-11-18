#include "texture2d.h"

Texture2D::Texture2D() {
    _id = 0;
    _width = 0;
    _height = 0;
    _format = TEXTURE_FORMAT_RGBA;
    _data = nullptr;
}

Texture2D::Texture2D(int width, int height, TextureFormat format, TextureFilter filter, TextureWrap wrap) {
    _id = 0;
    _width = width;
    _height = height;
    _format = format;
    _filter = filter;
    _wrap = wrap;

    int channels = 0;
    switch (format) {
        case TEXTURE_FORMAT_R:
            channels = 1;
            break;
        case TEXTURE_FORMAT_RG:
            channels = 2;
            break;
        case TEXTURE_FORMAT_RGB:
            channels = 3;
            break;
        case TEXTURE_FORMAT_RGBA:
            channels = 4;
            break;
        case TEXTURE_FORMAT_DEPTH:
            channels = 1;
            break;
    }

    _data = new unsigned char[width * height * channels];
    apply();
}

Texture2D::Texture2D(const std::string& path, const std::string& name) : Asset(path, name){
    _id = 0;
    //_data = stbi_load(path.c_str(), &_width, &_height, &_channels, 0);
}

Texture2D::~Texture2D() {
    if (_data != nullptr) {
        delete[] _data;
    }

    if (_id != 0) {
        glDeleteTextures(1, &_id);
    }
}

void Texture2D::apply() {
    if (_id == 0) {
        glGenTextures(1, &_id);
    }

    glBindTexture(GL_TEXTURE_2D, _id);
    glTexImage2D(GL_TEXTURE_2D, 0, _format, _width, _height, 0, _format, GL_UNSIGNED_BYTE, _data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    switch (_filter) {
        case TEXTURE_FILTER_NEAREST:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            break;
        case TEXTURE_FILTER_LINEAR:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
    }

    switch (_wrap) {
        case TEXTURE_WRAP_REPEAT:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            break;
        case TEXTURE_WRAP_CLAMP:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            break;
    }
}