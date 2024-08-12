#include "s_camera.h"

void cameraRender()
{
    Entity::each({COMPONENT_CAMERA,COMPONENT_TRANSFORM_2D},[&](Entity* entity) {
        Camera* camera = entity->getComponent<Camera>(COMPONENT_CAMERA);
        Transform2D* transform = entity->getComponent<Transform2D>(COMPONENT_TRANSFORM_2D);
        if(camera->isActive())
        {
            if(Window::current() != nullptr)
            {
                Window::current()->draw.setCamera(transform->position,transform->scale, transform->rotation);
            }
        }
    });
}