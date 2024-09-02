#ifndef POMEGRANATEENGINE_ASSET_MANAGER_H
#define POMEGRANATEENGINE_ASSET_MANAGER_H
#include "directory.h"
#include "file.h"
#include "asset.h"
#include "debug.h"

class AssetManager {
private:
    Directory _directory;
    List<Asset> _resources;
public:
    static void initializeDefaultDirectoryStructure();
};


#endif //POMEGRANATEENGINE_ASSET_MANAGER_H
