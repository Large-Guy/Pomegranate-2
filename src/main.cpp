#include<iostream>
#include<noise2d.h>
#include<window.h>

struct

struct Tilemap {
    int width, height;
    int* tiles;
};

int main() {
    Graphics::init();

    Window window;
    window.setTitle("Pomegranate Engine");
    window.setSize(800, 600);
    window.open();

    int resolution = 32;



    while(window.isOpen())
    {
        window.pollEvents();

        window.draw.begin();
        window.draw.clear();

        window.draw.end();
    }

    return 0;
}