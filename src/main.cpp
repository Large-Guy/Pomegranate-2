#include<iostream>
#include <vector2.h>
#include <entity.h>
#include <component.h>
#include <graphics_core.h>
#include <window.h>
#include <GLFW/glfw3.h>


int main() {

#define POSITION 0
#define VELOCITY 1
#define NAME 2

    new Archetype({POSITION});
    new Archetype({VELOCITY});
    new Archetype({POSITION,VELOCITY});
    new Archetype({POSITION,VELOCITY, NAME});

    std::unordered_set<archetype_id> position_archetypes = Archetype::_componentIndex[POSITION];
    std::unordered_set<archetype_id> velocity_archetypes = Archetype::_componentIndex[VELOCITY];

    for(auto& archetype : position_archetypes)
    {
        if(velocity_archetypes.count(archetype) != 0)
        {
            std::cout << "Found archetype with velocity and position: " << archetype << std::endl;
        }
    }

    return 0;
}