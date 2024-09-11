#include "hierarchy.h"

Children::Children() {
    this->children = std::unordered_set<Entity>();
    property("children", &this->children);
}

void Hierarchy::addChildTo(Entity parent, Entity child) {
    auto* children = parent.getComponent<Children>("Children");
    if(children == nullptr)
    {
        children = parent.addComponent<Children>("Children");
    }
    children->children.insert(child);
    if(child.hasComponent("Parent")) {
        child.getComponent<Parent>("Parent")->parent = parent;
    }
    else
    {
        child.addComponent<Parent>("Parent")->parent = parent;
    }
}

void Hierarchy::removeChildFrom(Entity parent, Entity child) {
    auto* children = parent.getComponent<Children>("Children");
    children->children.erase(child);
    child.getComponent<Parent>("Parent")->parent = NULL_ENTITY;
}

Parent::Parent() {
    this->parent = NULL_ENTITY;
    property("parent", &this->parent);
}

void Hierarchy::orphan(Entity entity) {
    auto* parent = entity.getComponent<Parent>("Parent");
    if(parent->parent.hasComponent("Children")) {
        auto* children = parent->parent.getComponent<Children>("Children");
        children->children.erase(entity);
    }
    parent->parent = NULL_ENTITY;
}

void Hierarchy::setParentOn(Entity entity, Entity new_parent) {
    auto* parent = entity.getComponent<Parent>("Parent");
    if(parent->parent.hasComponent("Children")) {
        auto* children = parent->parent.getComponent<Children>("Children");
        children->children.erase(entity);
    }
    parent->parent = new_parent;
    //Check if parent has children component
    if(new_parent.hasComponent("Children")) {
        auto* children = new_parent.getComponent<Children>("Children");
        children->children.insert(entity);
    }
    else
    {
        new_parent.addComponent<Children>("Children")->children.insert(entity);
    }
}
