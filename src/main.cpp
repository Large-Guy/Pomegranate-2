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

    return 0;
}