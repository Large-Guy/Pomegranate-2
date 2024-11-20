#ifndef POMEGRANATEENGINE_CAMERA3D_H
#define POMEGRANATEENGINE_CAMERA3D_H

#include <core/core.h>
#include <math/math.h>
#include <ecs/ecs.h>
#include <graphics/opengl/graphics.h>

class Camera3D : Reflectable {
private:
    static Entity _mainCamera;
public:
    float fov = 90.0f;
    float near = 0.01f;
    float far = 1000.0f;

    Camera3D();
    Camera3D(float fov, float near, float far);

    static void setMain(Entity& entity);
    static Entity getMain();
    bool isMain() const;
    Matrix4x4 getProjectionMatrix() const;

    void serialize(Archive& a) const override;
    void deserialize(Archive& a) override;
};


#endif //POMEGRANATEENGINE_CAMERA3D_H
