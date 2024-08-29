#include<iostream>
#include <utility>
#include <core/pomegranate.h>
#include <graphics/vulkan/graphics.h>

int main() {
    Graphics::init();
    Window window;
    window.show();
    window.setTitle("Pomegranate Engine - Vulkan");
    while(window.is_open())
    {
        window.poll();

    }
    return 0;
}