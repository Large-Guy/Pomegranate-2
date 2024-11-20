#ifndef POMEGRANATE_ENGINE_ECS_RENDERING_MESH_H
#define POMEGRANATE_ENGINE_ECS_RENDERING_MESH_H
#include <core/core.h>
#include <opengl/graphics.h>

class MeshInstance : Reflectable {
public:
    MeshBase* mesh;

    MeshInstance();
    MeshInstance(MeshBase* mesh);

    void serialize(Archive& a) const override;
    void deserialize(Archive& a) override;
};


#endif //POMEGRANATEENGINE_MESH_H
