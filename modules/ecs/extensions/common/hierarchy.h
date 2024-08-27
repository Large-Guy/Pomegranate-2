#ifndef POMEGRANATEENGINE_HIERARCHY_H
#define POMEGRANATEENGINE_HIERARCHY_H
#include <ecs/ecs.h>

struct Children : public Component {
    std::unordered_set<Entity> children;
    Children();
};

struct Parent : public Component {
    Entity parent;
    Parent();
};

struct Hierarchy {
    static void addChildTo(Entity parent, Entity child);
    static void removeChildFrom(Entity parent, Entity child);
    static void orphan(Entity entity);
    static void setParentOn(Entity entity, Entity new_parent);
};


#endif //POMEGRANATEENGINE_HIERARCHY_H
