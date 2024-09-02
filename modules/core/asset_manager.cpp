#include "asset_manager.h"

void AssetManager::initializeDefaultDirectoryStructure() {
    //Audio
    Directory sfxDirectory("assets/audio/sfx");
    sfxDirectory.init();
    Debug::Log::pass("Initialized SFX directory");

    Directory musicDirectory("assets/audio/music");
    musicDirectory.init();
    Debug::Log::pass("Initialized Music directory");


    //Graphics
    Directory texturesDirectory("assets/graphics/textures");
    texturesDirectory.init();
    Debug::Log::pass("Initialized Textures directory");

    Directory modelsDirectory("assets/graphics/models");
    modelsDirectory.init();
    Debug::Log::pass("Initialized Models directory");

    Directory shadersDirectory("assets/graphics/shaders");
    shadersDirectory.init();
    Debug::Log::pass("Initialized Shaders directory");

    Directory materialsDirectory("assets/graphics/materials");
    materialsDirectory.init();
    Debug::Log::pass("Initialized Materials directory");

    Directory animationsDirectory("assets/graphics/animations");
    animationsDirectory.init();
    Debug::Log::pass("Initialized Animations directory");

    //UI
    Directory fontsDirectory("assets/ui/fonts");
    fontsDirectory.init();
    Debug::Log::pass("Initialized Fonts directory");

    Directory imagesDirectory("assets/ui/images");
    imagesDirectory.init();
    Debug::Log::pass("Initialized Images directory");

    Directory stylesDirectory("assets/ui/styles");
    stylesDirectory.init();
    Debug::Log::pass("Initialized Styles directory");

    //Scripts
    Directory scriptsDirectory("assets/scripts");
    scriptsDirectory.init();
    Debug::Log::pass("Initialized Scripts directory");

    //Scenes
    Directory scenesDirectory("assets/scenes");
    scenesDirectory.init();
    Debug::Log::pass("Initialized Scenes directory");
}
