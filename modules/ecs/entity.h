#ifndef POMEGRANATEENGINE_ENTITY_H
#define POMEGRANATEENGINE_ENTITY_H
#include<string>
#include<unordered_map>
#include<vector>
#include<serializable.h>
#include"component.h"

class Entity : public Serializable {
private:
    uint id;
    std::string name;
public:
    Entity();
    explicit Entity(std::string name);
    [[nodiscard]] uint getId() const;
    [[nodiscard]] std::string getName() const;
    static uint getEntityCount();
    void serialize(Archive& a) const override;
    void deserialize(Archive& a) override;
};


#endif //POMEGRANATEENGINE_ENTITY_H
