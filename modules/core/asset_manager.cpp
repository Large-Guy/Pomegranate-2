#include "asset_manager.h"

void AssetManager::initializeDefaultDirectoryStructure() {
    //Audio
    Directory sfxDirectory("assets/audio/sfx");
    sfxDirectory.init();
    Directory musicDirectory("assets/audio/music");
    musicDirectory.init();

    //Graphics
    Directory texturesDirectory("assets/graphics/textures");
    texturesDirectory.init();
    Directory modelsDirectory("assets/graphics/models");
    modelsDirectory.init();
    Directory shadersDirectory("assets/graphics/shaders");
    shadersDirectory.init();
    Directory materialsDirectory("assets/graphics/materials");
    materialsDirectory.init();
    Directory animationsDirectory("assets/graphics/animations");
    animationsDirectory.init();

    //UI
    Directory fontsDirectory("assets/ui/fonts");
    fontsDirectory.init();
    Directory imagesDirectory("assets/ui/images");
    imagesDirectory.init();
    Directory stylesDirectory("assets/ui/styles");
    stylesDirectory.init();

    //Scripts
    Directory scriptsDirectory("assets/scripts");
    scriptsDirectory.init();

    //Scenes
    Directory scenesDirectory("assets/scenes");
    scenesDirectory.init();
}
