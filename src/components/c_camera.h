#ifndef POMEGRANATEENGINE_C_CAMERA_H
#define POMEGRANATEENGINE_C_CAMERA_H
#include <serializable.h>
#include <component.h>

extern component_id COMPONENT_CAMERA;

struct Camera {
private:
    static Camera* _current;
    bool _active = false;
public:
    void makeCurrent();
    bool isActive() const;
};


#endif //POMEGRANATEENGINE_C_CAMERA_H
