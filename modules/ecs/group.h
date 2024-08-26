#ifndef POMEGRANATEENGINE_GROUP_H
#define POMEGRANATEENGINE_GROUP_H
#include "entity.h"

class Group {
    std::string name;
    ComponentID component;
    std::unordered_set<EntityID> entities;
    static std::unordered_map<std::string, Group*> groups;
public:
    explicit Group(const std::string& name);
    ~Group();

    void addEntity(EntityID entity);
    void removeEntity(EntityID entity);
    bool hasEntity(EntityID entity);
    const std::unordered_set<EntityID>& getEntities() const;
    const std::string& getName() const;
    std::unordered_set<EntityID>::iterator begin() { return entities.begin(); }
    std::unordered_set<EntityID>::iterator end() { return entities.end(); }
};


#endif //POMEGRANATEENGINE_GROUP_H
