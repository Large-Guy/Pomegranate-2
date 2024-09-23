#include "hierarchy.h"

Children::Children() {
    this->children = std::unordered_set<Entity>();
    property("children", &this->children);
}

Children::Children(const Children& other) {
    this->children = other.children;
    property("children", &this->children);
}

void Hierarchy::addChildTo(Entity parent, Entity child) {
    auto* children = parent.get<Children>();
    if(children == nullptr)
    {
        children = parent.add<Children>();
    }
    children->children.insert(child);
    if(child.has("Parent")) {
        child.get<Parent>()->parent = parent;
    }
    else
    {
        child.add<Parent>()->parent = parent;
    }
}

void Hierarchy::removeChildFrom(Entity parent, Entity child) {
    auto* children = parent.get<Children>();
    children->children.erase(child);
    child.get<Parent>()->parent = NULL_ENTITY;
}

Parent::Parent() {
    this->parent = NULL_ENTITY;
    property("parent", &this->parent);
}

Parent::Parent(const Parent& other) {
    this->parent = other.parent;
    property("parent", &this->parent);
}

void Hierarchy::orphan(Entity entity) {
    auto* parent = entity.get<Parent>();
    if(parent->parent.has("Children")) {
        auto* children = parent->parent.get<Children>();
        children->children.erase(entity);
    }
    parent->parent = NULL_ENTITY;
}

void Hierarchy::setParentOn(Entity entity, Entity new_parent) {
    auto* parent = entity.get<Parent>();
    if(parent->parent.has("Children")) {
        auto* children = parent->parent.get<Children>();
        children->children.erase(entity);
    }
    parent->parent = new_parent;
    //Check if parent has children component
    if(new_parent.has("Children")) {
        auto* children = new_parent.get<Children>();
        children->children.insert(entity);
    }
    else
    {
        new_parent.add<Children>()->children.insert(entity);
    }
}
