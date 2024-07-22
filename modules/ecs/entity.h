#ifndef POMEGRANATEENGINE_ENTITY_H
#define POMEGRANATEENGINE_ENTITY_H
#include<string>
#include<unordered_map>
#include<vector>
#include<serializable.h>
#include<component.h>
#include<archetype.h>
#include <ecs_typedefs.h>

class Entity : public Serializable {
private:
    EntityID _id;
    std::string _name;
    std::unordered_map<ComponentID,std::any> _components;

    static uint _entityCount;
public:
    static std::unordered_map<EntityID,Archetype*> entities;

    Entity();
    explicit Entity(std::string name);

    // Getters
    [[nodiscard]] uint getId() const;
    [[nodiscard]] std::string getName() const;

    // Setters

    void setName(const std::string& name);

    // Component

    bool hasComponent(ComponentID c);
    void addComponent(ComponentID c, std::any component);

    // Operators

    bool operator==(const Entity& e) const;
    bool operator!=(const Entity& e) const;

    void serialize(Archive& a) const override;
    void deserialize(Archive& a) override;

    static uint getEntityCount();
};


#endif //POMEGRANATEENGINE_ENTITY_H
