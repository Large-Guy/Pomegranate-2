#ifndef POMEGRANATEENGINE_MATERIAL_H
#define POMEGRANATEENGINE_MATERIAL_H
#include <core/core.h>
#include <opengl/graphics.h>

class Material : Reflectable {
public:
    ShaderBase* shader;

    Material();
    Material(ShaderBase* shader);

    void serialize(Archive& a) const override;
    void deserialize(Archive& a) override;
};


#endif //POMEGRANATEENGINE_MATERIAL_H
