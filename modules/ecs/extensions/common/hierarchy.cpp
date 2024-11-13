#include "hierarchy.h"

Children::Children() {
    this->children = std::unordered_set<Entity>();
    property("children", &this->children);
}

Children::Children(const Children& other)  : Reflectable(other) {
    this->children = other.children;
    property("children", &this->children);
}

void Children::serialize(Archive& a) const {
    a << children.size();
    for(auto& child : children) {
        a << child.id;
    }
}

void Children::deserialize(Archive& a) {
    size_t size = 0;
    a >> size;
    for(size_t i = 0; i < size; i++) {
        EntityID id = 0;
        a >> id;
        children.insert(Entity(id));
    }
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

void Parent::serialize(Archive& a) const {
    a << parent.id;
}

void Parent::deserialize(Archive& a) {
    EntityID id = 0;
    a >> id;
    parent = Entity(id);
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
