#include "tile_map.h"

void tileMapGenerate(TileMap* tileMap,TileSet* tileSet)
{
    for(auto& model : tileMap->models) {
        model.second->getVertices().clear();
        model.second->getIndices().clear();
    }

    for (int y = 0; y < tileMap->height; y++) {
        for (int x = 0; x < tileMap->width; x++) {
            unsigned int tile = tileMap->getTile(x, y);

            if (tile == 0) continue;

            tile -= 1;
            if (tileSet->inBounds(tile)) {
                Tile &tileData = tileSet->getTile(tile);
                if(tileMap->models.find(tileData.texture) == tileMap->models.end()) {
                    tileMap->models[tileData.texture] = new Model2D();
                }
                Model2D* model = tileMap->models[tileData.texture];

                std::vector<Vertex2D>& vertices = model->getVertices();
                std::vector<unsigned int>& indices = model->getIndices();

                Rect rect = tileData.rect;
                Vector2 origin = tileData.origin;
                //Render tile

                unsigned int i = vertices.size();
                indices.insert(indices.end(), {i + 2, i + 1, i, i + 3, i + 2, i});

                vertices.push_back({
                                           {(float) x,(float) y},
                                           {1.0f},
                                           {rect.position.x, rect.position.y}
                                   });
                vertices.push_back({
                                           {(float) x,(float) y + 1},
                                           {1.0f},
                                           {rect.position.x, rect.position.y + rect.size.y}
                                   });
                vertices.push_back({
                                           {(float) x + 1,(float) y + 1},
                                           {1.0f},
                                           {rect.position.x + rect.size.x, rect.position.y + rect.size.y}
                                   });
                vertices.push_back({
                                           {(float) x + 1,(float) y},
                                           {1.0f},
                                           {rect.position.x + rect.size.x, rect.position.y}
                                   });
            }
        }
    }



    tileMap->modelNeedsBuilding = true;
    tileMap->changes = false;
    tileMap->generationRunning = false;
}

void tileMapRender(void*)
{
    Group* group = Group::find("world");
    if(group == nullptr) return;
    group->each({TILE_SET,TILE_MAP}, [&](Entity* entity){
        auto* tileSet = entity->getComponent<TileSet>(TILE_SET);

        Transform2D* transform = nullptr;
        if(entity->hasComponent(TRANSFORM_2D))
            transform = entity->getComponent<Transform2D>(TRANSFORM_2D);


        auto* tileMap = entity->getComponent<TileMap>(TILE_MAP);


        if(!tileMap->generationRunning && tileMap->changes) {
            tileMap->generationRunning = true;
            if(tileMap->generationThread != nullptr)
            {
                if(tileMap->generationThread->joinable())
                    tileMap->generationThread->join();
                delete tileMap->generationThread;
            }
            tileMap->generationThread = new std::thread(tileMapGenerate,tileMap,tileSet);
        }

        if(tileMap->modelNeedsBuilding && !tileMap->generationRunning) {
            for(auto& model : tileMap->models) {
                model.second->regenerateBuffers();
            }
            tileMap->modelNeedsBuilding = false;
        }

        for (auto &model: tileMap->models) {
            if (tileMap->shader == nullptr) throw std::runtime_error("TileMap shader is null");
            model.first->bind(0);
            tileMap->shader->use();
            tileMap->shader->set("SCREEN_RESOLUTION", Vector2((float) Graphics::getViewportWidth(),
                                                              (float) Graphics::getViewportHeight()));
            tileMap->shader->set("TEXTURE", model.first);
            if (transform != nullptr) {
                tileMap->shader->set("MODEL_MATRIX", transform->getMatrix());
            } else {
                tileMap->shader->set("MODEL_MATRIX", Matrix3x3::createTransform(Vector2(0, 0), Vector2(64, 64), 0));
            }
            tileMap->shader->set("Z_INDEX", 0.0f);
            model.second->draw();
        }
    });
}