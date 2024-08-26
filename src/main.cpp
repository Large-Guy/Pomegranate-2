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

struct Vector2Hash
{
    std::size_t operator()(const Vector2& vector) const
    {
        std::size_t hash = 0;
        hash ^= std::hash<float>()(vector.x) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        hash ^= std::hash<float>()(vector.y) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        return hash;
    }
};

int main() {
    Table table = Table();
    table.set<std::string,float>("Speed", 10.0f);
    table.set<int,std::string>(1, "Hello");
    table.set<int,std::string>(2, "World");
    table.set<Vector2, std::string, Vector2Hash>(Vector2(1,2), "Vector2");

    std::cout << table.get<std::string,float>("Speed") << std::endl;
    std::cout << table.get<int,std::string>(1) << std::endl;
    std::cout << table.get<int,std::string>(2) << std::endl;
    std::cout << table.get<Vector2,std::string,Vector2Hash>(Vector2(1,2)) << std::endl;

    return 0;
}