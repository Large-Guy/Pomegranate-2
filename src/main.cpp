#include <utility>
#include <core/core.h>
#include <graphics/vulkan/graphics.h>
#include <ecs/ecs.h>
#include <ecs/extensions/common/common.h>
#include <math/math.h>
#include <thread>
#include <omp.h>

int main() {
    Graphics::init();
    ECS::setThreadCount(ECS::getMaxThreadCount()/2);
    AssetManager::initializeDefaultDirectoryStructure();
    Extensions::Common::init();

    Entity entity = Entity::create();
    entity.addComponent<Transform3D>("Transform3D", Vector3(0, 0, 0), Vector3(1, 1, 1), Vector3(0, 0, 0));
    entity.addComponent<Name>("Name", "Entity");
    return 0;
}