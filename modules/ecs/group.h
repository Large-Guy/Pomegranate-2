#ifndef POMEGRANATEENGINE_GROUP_H
#define POMEGRANATEENGINE_GROUP_H
#include <vector>
#include <entity.h>
#include <archetype.h>

// Group is a component
struct Group
{
private:
    static std::unordered_map<std::string, Group*> _groupIndex;
    std::string _name = "";
    std::vector<Entity*> _entities = {};
public:

    void addEntity(Entity* entity);
    void removeEntity(Entity* entity);
    Entity* getEntity(entity_id id);
    void setName(const std::string& name);

    template<typename Func> void each(const std::vector<component_id>& components,Func&& func) {
        for(int i = 0; i < _entities.size(); i++) {
            Entity* entity = _entities[i];
            bool hasComponents = true;
            for(int j = 0; j < components.size(); j++) {
                if(!entity->hasComponent(components[j])) {
                    hasComponents = false;
                    break;
                }
            }
            if(hasComponents) {
                func(entity);
            }
        }
    }

    static Group* getGroup(const std::string& name) {
        if (_groupIndex.find(name) == _groupIndex.end()) {
            return nullptr;
        }
        return _groupIndex[name];
    }
};

#endif //POMEGRANATEENGINE_GROUP_H
