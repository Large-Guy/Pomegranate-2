#include "tile_map.h"

void tileMapGenerate(TileMap* tileMap,TileSet* tileSet)
{
    for(auto& dirty : tileMap->dirty)
    {
        chunk_id chunk = dirty.first;
        for(auto& tile : dirty.second)
        {
            Tile& tileData = tileSet->getTile(tile);
            if(tileMap->models.find(chunk) == tileMap->models.end()) {
                tileMap->models.insert({});
            }
            if(tileMap->models[chunk].find(tile) == tileMap->models[chunk].end())
            {
                tileMap->models[chunk][tile] = new Model2D();
            }
            Model2D*& model = tileMap->models[chunk][tile];
            if(model == nullptr)
            {
                model = new Model2D();
            }

            std::vector<Vertex2D>& vertices = model->getVertices();
            std::vector<unsigned int>& indices = model->getIndices();
            vertices.clear();
            indices.clear();

            vertices.resize(tileMap->tiles[chunk][tile].size() * 4);
            indices.resize(tileMap->tiles[chunk][tile].size() * 6);

            int vertexCount = 0;
            int indiciesCount = 0;

            for(auto& position : tileMap->tiles[chunk][tile])
            {

                Rect rect = tileData.rect;
                Vector2 origin = tileData.origin;
                //Render tile

                unsigned int i = vertexCount;
                indices[indiciesCount] = i + 2;
                indices[indiciesCount + 1] = i + 1;
                indices[indiciesCount + 2] = i;
                indices[indiciesCount + 3] = i + 3;
                indices[indiciesCount + 4] = i + 2;
                indices[indiciesCount + 5] = i;

                vertices[vertexCount] = {
                                           {(float) position.x,(float) position.y},
                                           {1.0f},
                                           {rect.position.x, rect.position.y}
                                   };
                vertices[vertexCount+1] = {
                                           {(float) position.x,(float) position.y + 1},
                                           {1.0f},
                                           {rect.position.x, rect.position.y + rect.size.y}
                                   };
                vertices[vertexCount+2] = {
                                           {(float) position.x + 1,(float) position.y + 1},
                                           {1.0f},
                                           {rect.position.x + rect.size.x, rect.position.y + rect.size.y}
                                   };
                vertices[vertexCount+3] = {
                                           {(float) position.x + 1,(float) position.y},
                                           {1.0f},
                                           {rect.position.x + rect.size.x, rect.position.y}
                                   };
                vertexCount += 4;
                indiciesCount += 6;
            }
        }
    }

    tileMap->modelNeedsBuilding = true;
}

void tileMapRender(void*)
{
    Group* group = Group::find("world");
    if(group == nullptr) return;
    group->each({COMPONENT_TILE_SET,COMPONENT_TILE_MAP}, [&](Entity* entity){
        auto* tileSet = entity->getComponent<TileSet>(COMPONENT_TILE_SET);

        Transform2D* transform = nullptr;
        if(entity->hasComponent(COMPONENT_TRANSFORM_2D))
            transform = entity->getComponent<Transform2D>(COMPONENT_TRANSFORM_2D);


        auto* tileMap = entity->getComponent<TileMap>(COMPONENT_TILE_MAP);

        if(tileMap->modelNeedsBuilding) {
            for(auto& chunk : tileMap->dirty) {
                for(auto& tile : chunk.second)
                {
                    if(tileMap->models[chunk.first][tile] != nullptr)
                        tileMap->models[chunk.first][tile]->regenerateBuffers();
                    else
                        std::cout << "Model null" << std::endl;
                }
            }
            tileMap->modelNeedsBuilding = false;
            tileMap->dirty.clear();
        }

        if(!tileMap->dirty.empty()) {
            tileMapGenerate(tileMap,tileSet);
        }

        for (auto &chunk: tileMap->models) {
            for(auto& model : chunk.second) {
                if (tileMap->shader == nullptr) throw std::runtime_error("TileMap shader is null");
                Texture2D* texture = tileSet->getTile(model.first).texture;
                texture->bind(0);
                tileMap->shader->use();
                tileMap->shader->set("SCREEN_RESOLUTION", Vector2((float) Graphics::getViewportWidth(),
                                                                  (float) Graphics::getViewportHeight()));
                tileMap->shader->set("TEXTURE", texture);
                if (transform != nullptr) {
                    tileMap->shader->set("MODEL_MATRIX", transform->getMatrix().translate(Vector2(chunk.first.x * CHUNK_SIZE * transform->scale.x,chunk.first.y * CHUNK_SIZE * transform->scale.y)));
                } else {
                    tileMap->shader->set("MODEL_MATRIX", Matrix3x3::createTransform(Vector2(0, 0), Vector2(64, 64), 0));
                }
                tileMap->shader->set("Z_INDEX", 0.0f);
                if(model.second != nullptr)
                    model.second->draw();
                else
                    std::cout << "Model null" << std::endl;
            }
        }
    });
}