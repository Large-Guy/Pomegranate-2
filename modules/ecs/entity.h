#ifndef POMEGRANATEENGINE_ENTITY_H
#define POMEGRANATEENGINE_ENTITY_H
#include<string>
#include<unordered_map>
#include<vector>

class Entity {
private:
    uint id;
    std::string name;
    static uint entityCount;
    static std::unordered_map<uint, Entity*> entities;
public:
    Entity();
    int getId() const;
    static int getEntityCount();
};


#endif //POMEGRANATEENGINE_ENTITY_H
