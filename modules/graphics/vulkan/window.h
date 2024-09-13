#ifndef POMEGRANATEENGINE_WINDOW_H
#define POMEGRANATEENGINE_WINDOW_H
#include "core.h"
#include <core/core.h>
#include <math/math.h>
#include "vertex2d.h"
#include "vertex3d.h"


template<BufferType Type> class BufferBase;
template <typename T, BufferType BufferType> class Buffer;
struct Mesh3D;

class Window {
public:
    class Draw {
    public:
        Window* window;
        uint32_t imageIndex;
        void begin();
        void end();
        void clear(Vector4 color);
        void buffers(BufferBase<BUFFER_TYPE_VERTEX>* vertexBuffer, BufferBase<BUFFER_TYPE_INDEX>* indexBuffer, ShaderBase* shader);
        void mesh(Mesh3D& mesh);
    };

    GLFWwindow* _window;
    VkSurfaceKHR _surface;
    VkSwapchainKHR _swapChain;
    std::vector<VkImage> _swapChainImages;
    std::vector<VkImageView> _swapChainImageViews;
    VkFormat _swapChainImageFormat;
    VkExtent2D _swapExtent;
    std::vector<VkFramebuffer> _swapChainFramebuffers;

    VkRenderPass _renderPass;

    std::vector<VkCommandBuffer> _commandBuffers;

    uint32_t _currentFrame = 0;

    String _title;
    Vector2i _size;
    bool _fullscreen;
    bool _visible;
    bool _open;
    Vector2i _position;

    void createSwapChain();
    void createImageViews();
    VkExtent2D getExtents(const VkSurfaceCapabilitiesKHR& capabilities);
public:
    Draw draw;
    Window();
    ~Window();

    void createFrameBuffers();
    void createCommandBuffer();
    void beginCommandBuffer();
    void endCommandBuffer();
    VkCommandBuffer& getCurrentCommandBuffer();

    void setTitle(const String& title);
    void setSize(Vector2i size);
    void setSize(int width, int height);
    void poll();
    void show();
    void hide();
    void fullscreen();
    void maximize();
    void minimize();
    void focus();
    void setPosition(Vector2i position);
    void setPosition(int x, int y);

    Vector2i getPosition() const;
    Vector2i getSize() const;
    String getTitle() const;
    [[nodiscard]] bool isOpen() const;

    friend Graphics;
};


#endif //POMEGRANATEENGINE_WINDOW_H
