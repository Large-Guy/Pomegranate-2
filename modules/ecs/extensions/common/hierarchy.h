#ifndef POMEGRANATE_ENGINE_ECS_EXTENSIONS_COMMON_HIERARCHY_H
#define POMEGRANATE_ENGINE_ECS_EXTENSIONS_COMMON_HIERARCHY_H
#include <ecs/ecs.h>

struct Children : public Reflectable {
    std::unordered_set<Entity> children;
    Children();
};

struct Parent : public Component, public Reflectable {
    Entity parent;
    Parent();
};

struct Hierarchy {
    static void addChildTo(Entity parent, Entity child);
    static void removeChildFrom(Entity parent, Entity child);
    static void orphan(Entity entity);
    static void setParentOn(Entity entity, Entity new_parent);
};


#endif //POMEGRANATE_ENGINE_ECS_EXTENSIONS_COMMON_HIERARCHY_H
