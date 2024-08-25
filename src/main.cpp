#include<iostream>
#include <core/pomegranate.h>
#include <ecs/ecs.h>
#include <ecs/entity.h>
#include <math/pmath.h>
#include <graphics/glfw/graphics.h>
#include <events/events.h>
#include <core/list.h>

int main() {
    List<int> list = range(0,16);
    std::cout << list << std::endl;
    SERIALIZE_TO_FILE(list, "list.bin");
    List<int> list2;
    DESERIALIZE_FROM_FILE(list2, "list.bin");
    std::cout << list2 << std::endl;
    return 0;
}