#ifndef POMEGRANATEENGINE_TEXTURE_H
#define POMEGRANATEENGINE_TEXTURE_H
#include <core/resource.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math/vector4.h>
#include <math/vector2.h>

enum class TextureFilter {
    TEXTURE_FILTER_NEAREST = GL_NEAREST,
    TEXTURE_FILTER_LINEAR = GL_LINEAR,
    TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
    TEXTURE_FILTER_LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
    TEXTURE_FILTER_NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
    TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
};

class Texture2D : public Resource {
private:
    GLuint _id = 0;
    int _width = 0;
    int _height = 0;
    int _channels = 0;
    unsigned char* _data = nullptr;
    uint _slot = 0;
public:
    TextureFilter filter = TextureFilter::TEXTURE_FILTER_LINEAR;
    explicit Texture2D(const std::string& path, std::string name = "");
    Texture2D(int width, int height, int channels = 4);
    Texture2D(const Texture2D& other) = delete;
    Texture2D& operator=(const Texture2D& other) = delete;
    ~Texture2D() override;
    void reload() override;
    void bind(uint slot = 0);
    bool apply();

    [[nodiscard]] GLuint getGLTexture() const;
    [[nodiscard]] uint getBindSlot() const;
    [[nodiscard]] int getWidth() const;
    [[nodiscard]] int getHeight() const;
    [[nodiscard]] int getChannels() const;
    [[nodiscard]] unsigned char* getData() const;

    void setData(unsigned char* data);
    void setPixel(int x, int y, Vector4 color);
    [[nodiscard]] Vector4 getPixel(int x, int y) const;

    void serialize(Archive &a) const override;
    void deserialize(Archive &a) override;
};


#endif //POMEGRANATEENGINE_TEXTURE_H
