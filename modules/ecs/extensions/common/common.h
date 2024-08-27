#ifndef POMEGRANATEENGINE_COMMON_H
#define POMEGRANATEENGINE_COMMON_H

#include "transform2d.h"
#include "transform3d.h"
#include "name.h"
#include "color.h"
#include "hierarchy.h"

void registerCommonComponents() {
    ECS::registerComponent<Transform2D>("Transform2D");
    ECS::registerComponent<Transform3D>("Transform3D");
    ECS::registerComponent<Name>("Name");
    ECS::registerComponent<Color>("Color");
    ECS::registerComponent<Children>("Children");
    ECS::registerComponent<Parent>("Parent");
}

#endif //POMEGRANATEENGINE_COMMON_H
