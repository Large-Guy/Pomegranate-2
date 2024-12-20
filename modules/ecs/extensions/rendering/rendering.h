#ifndef POMEGRANATE_ENGINE_ECS_RENDERING_H
#define POMEGRANATE_ENGINE_ECS_RENDERING_H

#include "mesh.h"
#include "material.h"
#include "camera3d.h"
#include "ecs/extensions/common/common.h"

namespace Extensions::Rendering {
    ComponentID MESH_INSTANCE;
    ComponentID MATERIAL;
    ComponentID CAMERA;
    void init() {
        MESH_INSTANCE = Component::create<MeshInstance>("MeshInstance");
        MATERIAL = Component::create<Material>("Material");
        CAMERA = Component::create<Camera3D>("Camera");
    }

    void render3d()
    {
        Camera3D* camera = nullptr;
        Entity camEntity = Camera3D::getMain();
        if(camEntity != NULL_ENTITY && camEntity.has<Camera3D>())
        {
            camera = camEntity.get<Camera3D>();
        }
        else
        {
            Debug::Log::warn("No main camera!");
            return;
        }

        Matrix4x4 view = Matrix4x4::identity().translate(Transform3D::getPosition(camEntity)).rotate(Transform3D::getRotation(camEntity));
        Matrix4x4 projection = camera->getProjectionMatrix();

        ECS::each<MeshInstance>(MESH_INSTANCE, [&](MeshInstance* meshInstance, Entity& entity){
            ShaderBase* shader = nullptr;
            if(entity.has<Material>())
            {
                Material* material = entity.get<Material>();
                shader = material->shader;
            }
            else
            {
                Debug::Log::warn("Entity does not have a material!");
                return;
            }

            Matrix4x4 model = Matrix4x4::identity();
            if(entity.has<Transform3D>())
            {
                model = Transform3D::getMatrix(entity);
            }

            Window::getCurrent()->draw.shader(shader);
            shader->setUniform("projection", projection);
            shader->setUniform("view", view);
            shader->setUniform("model", model);
            Window::getCurrent()->draw.mesh(meshInstance->mesh);
        });
    }
}

#endif //POMEGRANATEENGINE_RENDERING_H
