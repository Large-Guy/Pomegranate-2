#include<iostream>
#include <core/pomegranate.h>
#include <ecs/ecs.h>
#include <ecs/entity.h>
#include <math/pmath.h>
#include <graphics/glfw/graphics.h>
#include <events/events.h>
#include <core/list.h>
#include <core/hash_table.h>

size_t hash(Vector2 v)
{
    return v.x + v.y;
}

int main() {
    HashTable<std::string, std::string> table = HashTable<std::string, std::string>();

    table["hello"] = "Hello, ";
    table["world"] = "World!";

    table.toFile("table.bin");

    table.fromFile("table.bin");

    std::cout << table["hello"] << table["world"] << std::endl;
    return 0;
}