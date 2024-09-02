#ifndef POMEGRANATE_ENGINE_ECS_EXTENSIONS_COMMON_COMMON_H
#define POMEGRANATE_ENGINE_ECS_EXTENSIONS_COMMON_COMMON_H

#include "transform2d.h"
#include "transform3d.h"
#include "name.h"
#include "color.h"
#include "hierarchy.h"

namespace Extensions::Common {
    ComponentID TRANSFORM_2D = 0;
    ComponentID TRANSFORM_3D = 0;
    ComponentID NAME = 0;
    ComponentID COLOR = 0;
    ComponentID CHILDREN = 0;
    ComponentID PARENT = 0;
    void init() {
        TRANSFORM_2D = Component::create<Transform2D>("Transform2D");
        TRANSFORM_3D = Component::create<Transform3D>("Transform3D");
        NAME = Component::create<Name>("Name");
        COLOR = Component::create<Color>("Color");
        CHILDREN = Component::create<Children>("Children");
        PARENT = Component::create<Parent>("Parent");
    }
}

#endif //POMEGRANATE_ENGINE_ECS_EXTENSIONS_COMMON_COMMON_H
