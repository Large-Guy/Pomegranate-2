#include "texture.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include <stb_image.h>

Texture2D::Texture2D(const std::string& path, std::string name) : Resource(path, name) {
    _id = -1;

    if(path.empty()) {
        return;
    }

    stbi_set_flip_vertically_on_load(true);
    _data = stbi_load(path.c_str(), &_width, &_height, &_channels, 0);

    if(!apply())
    {
        std::cerr << "Failed to load texture: " << path << std::endl;
    }
}

Texture2D::Texture2D(int width, int height, int channels) {
    _id = -1;
    _width = width;
    _height = height;
    _channels = channels;
    _data = new unsigned char[width * height * channels];
    apply();
}

Texture2D::~Texture2D() {
    glDeleteTextures(1, &_id);
    stbi_image_free(_data);
}

void Texture2D::reload() {
    glBindTexture(GL_TEXTURE_2D, _id);

    stbi_set_flip_vertically_on_load(true);
    _data = stbi_load(getPath().c_str(), &_width, &_height, &_channels, 0);

    if(!apply())
    {
        std::cerr << "Failed to reload texture: " << getPath() << std::endl;
    }
}

GLuint Texture2D::getGLTexture() const {
    return _id;
}

void Texture2D::serialize(Archive &a) const {
    a << _width;
    a << _height;
    a << _channels;
    //Convert data to string
    std::string dataStr(reinterpret_cast<char*>(_data), _width * _height * _channels);
    a << dataStr;
}

void Texture2D::deserialize(Archive &a) {
    a >> &_width;
    a >> &_height;
    a >> &_channels;
    //Convert string to data
    std::string dataStr;
    a >> &dataStr;
    _data = reinterpret_cast<unsigned char*>(const_cast<char*>(dataStr.c_str()));

    if(!apply())
    {
        std::cerr << "Failed to deserialize texture: " << getName() << std::endl;
    }
}

void Texture2D::bind(uint slot) {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, _id);
    this->_slot = slot;
}

uint Texture2D::getBindSlot() const {
    return _slot;
}

int Texture2D::getWidth() const {
    return _width;
}

int Texture2D::getHeight() const {
    return _height;
}

bool Texture2D::apply() {
    if(_id != 0)
    {
        glDeleteTextures(1, &_id);
    }
    if (_data) {

        glGenTextures(1, &_id);
        glBindTexture(GL_TEXTURE_2D, _id);

        GLenum format;
        if (_channels == 1) {
            format = GL_RED;
        } else if (_channels == 3) {
            format = GL_RGB;
        } else if (_channels == 4) {
            format = GL_RGBA;
        }
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, format, _width, _height, 0, format, GL_UNSIGNED_BYTE, _data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        return true;
    } else {
        return false;
    }
}

void Texture2D::setPixel(int x, int y, Vector4 color) {
    if(_data) {
        int index = (x + y * _width) * _channels;
        _data[index] = (unsigned char)(color.x * 255);
        _data[index + 1] = (unsigned char)(color.y * 255);
        _data[index + 2] = (unsigned char)(color.z * 255);
        if(_channels == 4) {
            _data[index + 3] = (unsigned char)(color.w * 255);
        }
        apply();
    }
}

Vector4 Texture2D::getPixel(int x, int y) const {
    if(_data) {
        int index = (x + y * _width) * _channels;
        return {_data[index] / 255.0f, _data[index + 1] / 255.0f, _data[index + 2] / 255.0f, _channels == 4 ? _data[index + 3] / 255.0f : 1.0f};
    }
    return {0, 0, 0, 0};
}