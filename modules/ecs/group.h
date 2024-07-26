#ifndef POMEGRANATEENGINE_GROUP_H
#define POMEGRANATEENGINE_GROUP_H
#include <vector>
#include <entity.h>
#include <archetype.h>

class Group {
private:
    std::unordered_map<entity_id, Entity*> _entities;
public:
    Group();
    ~Group();

    void addEntity(Entity* entity);
    void removeEntity(entity_id id);
    void removeEntity(Entity* entity);
    bool hasEntity(entity_id id);
    bool hasEntity(Entity* entity);

    Entity* getEntity(entity_id id);

    std::vector<Entity*> getEntities();
    std::vector<Entity*> getEntities(const entity_type& type);

    template<typename Func> void each(const entity_type& type, Func func);
};

template<typename Func>
void Group::each(const entity_type& type, Func func) {
    //Sort type
    entity_type type_copy = type;
    std::sort(type_copy.begin(), type_copy.end());
    for(auto& e : this->_entities)
    {
        if(hasEntity(e.first)) {
            if (Entity::_entityIndex[e.first].archetype->_type == type) {
                func(e.second);
            }
        }
    }
}

#endif //POMEGRANATEENGINE_GROUP_H
