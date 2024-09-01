#ifndef POMEGRANATEENGINE_COMMON_H
#define POMEGRANATEENGINE_COMMON_H

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

#endif //POMEGRANATEENGINE_COMMON_H
