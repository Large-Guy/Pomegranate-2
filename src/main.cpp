#include<iostream>
#include <core/pomegranate.h>
#include <ecs/ecs.h>
#include <ecs/entity.h>
#include <math/pmath.h>
#include <graphics/glfw/graphics.h>
#include <events/events.h>
#include <core/list.h>
#include <core/hash_table.h>
#include <core/table.h>

int main() {
    Table table = Table();
    table.set<std::string,float>("Speed", 10.0f);
    table.set<int,std::string>(1, "Hello");
    table.set<int,std::string>(2, "World");
    table.set<Vector2, std::string>(Vector2(1,2), "Vector2");

    std::cout << table.get<std::string,float>("Speed") << std::endl;
    std::cout << table.get<int,std::string>(1) << std::endl;
    std::cout << table.get<int,std::string>(2) << std::endl;
    std::cout << table.get<Vector2,std::string>(Vector2(1,2)) << std::endl;

    return 0;
}