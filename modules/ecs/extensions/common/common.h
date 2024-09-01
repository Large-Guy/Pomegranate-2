#ifndef POMEGRANATE_ENGINE_ECS_EXTENSIONS_COMMON_COMMON_H
#define POMEGRANATE_ENGINE_ECS_EXTENSIONS_COMMON_COMMON_H

#include "transform2d.h"
#include "transform3d.h"
#include "name.h"
#include "color.h"
#include "hierarchy.h"

void registerCommonComponents() {
    Component::create<Transform2D>("Transform2D");
    Component::create<Transform3D>("Transform3D");
    Component::create<Name>("Name");
    Component::create<Color>("Color");
    Component::create<Children>("Children");
    Component::create<Parent>("Parent");
}

#endif //POMEGRANATE_ENGINE_ECS_EXTENSIONS_COMMON_COMMON_H
