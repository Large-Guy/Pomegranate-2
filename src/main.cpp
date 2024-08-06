#include<iostream>
#include<window.h>
#include<rect.h>
#include<group.h>
#include<event_manager.h>

Shader* tileMapShader = nullptr;

struct Tile : public Serializable
{
    Rect rect = Rect();
    Vector2 origin = Vector2();

    Tile() {
        rect = Rect();
        origin = Vector2();
    }

    Tile(Rect rect, Vector2 origin) {
        this->rect = rect;
        this->origin = origin;
    }

    void serialize(Archive& a) const override {
        a << rect << origin;
    }

    void deserialize(Archive& a) override {
        a >> &rect >> &origin;
    }
};

#define TILE_SET 0
struct TileSet : public Serializable {
    std::vector<Tile> tiles = {};

    TileSet() {

    }

    TileSet(const TileSet& tileSet) {
        tiles = tileSet.tiles;
    }

    void addTile(const Tile& tile) {
        tiles.push_back(tile);
    }

    bool inBounds(unsigned int index) const {
        return index < tiles.size();
    }

    Tile& getTile(unsigned int index) {
        return tiles[index];
    }

    void serialize(Archive& a) const override {
        a << tiles.size();
        for(auto& tile : tiles) {
            a << tile;
        }
    }

    void deserialize(Archive& a) override {
        int size;
        a >> &size;
        for(int i = 0; i < size; i++) {
            Tile tile;
            a >> &tile;
            tiles.push_back(tile);
        }
    }
};

#define TILE_MAP 1
typedef unsigned int tile_id;
struct TileMap : public Serializable {
    int width = 0, height = 0;
    tile_id* tiles = nullptr;
    Model2D* model = nullptr;
    bool changes = false;

    TileMap() {
        tiles = nullptr;
        width = 0;
        height = 0;
        model = nullptr;
        changes = false;
    }

    void resize(int width, int height) {
        this->width = width;
        this->height = height;
        if(tiles != nullptr) delete[] tiles;
        tiles = new tile_id[width * height];
        changes = true;
    }

    bool inBounds(int x, int y) {
        return x >= 0 && x < width && y >= 0 && y < height;
    }

    void setTile(int x, int y, unsigned int tile) {
        if(!inBounds(x, y)) return;
        tiles[y * width + x] = tile;
        changes = true;
    }

    unsigned int getTile(int x, int y) {
        if(!inBounds(x, y)) return 0;
        return tiles[y * width + x];
    }

    void serialize(Archive& a) const override {
        a << width << height;
        for(int i = 0; i < width * height; i++) {
            a << tiles[i];
        }
    }

    void deserialize(Archive& a) override {
        a >> &width >> &height;
        //tiles = std::vector<tile_id>(width * height);
        for(int i = 0; i < width * height; i++) {
            a >> &tiles[i];
        }
    }
};

void tileMapRender(void*)
{
    Group* group = Group::find("world");
    if(group == nullptr) return;
    group->each({TILE_SET,TILE_MAP}, [&](Entity* entity){
        auto* tileMap = entity->getComponent<TileMap>(TILE_MAP);
        auto* tileSet = entity->getComponent<TileSet>(TILE_SET);

        auto*& model = tileMap->model;

        if(tileMap->changes) {

            std::vector<Vertex2D> vertices;
            std::vector<unsigned int> indices;
            if(model != nullptr) delete model;
            if(model == nullptr || tileMap->changes) {
                model = new Model2D();
            }

            for (int y = 0; y < tileMap->height; y++) {
                for (int x = 0; x < tileMap->width; x++) {
                    unsigned int tile = tileMap->getTile(x, y);

                    if (tile == 0) continue;

                    tile -= 1;
                    if (tileSet->inBounds(tile)) {
                        Tile &tileData = tileSet->getTile(tile);
                        Rect rect = tileData.rect;
                        Vector2 origin = tileData.origin;
                        //Render tile
                        std::cout << "Render Tile at: " << x << "," << y << std::endl;

                        unsigned int i = vertices.size();
                        indices.insert(indices.end(), {i, i + 1, i + 2, i + 2, i + 3, i});

                        vertices.push_back({
                                                   {(float) x,       (float) y},
                                                   {1.0f,            1.0f, 1.0f},
                                                   {rect.position.x, rect.position.y}
                                           });
                        vertices.push_back({
                                                   {(float) x,       (float) y + 1},
                                                   {1.0f,            1.0f, 1.0f},
                                                   {rect.position.x, rect.position.y + rect.size.y}
                                           });
                        vertices.push_back({
                                                   {(float) x + 1,                 (float) y + 1},
                                                   {1.0f,                          1.0f, 1.0f},
                                                   {rect.position.x + rect.size.x, rect.position.y + rect.size.y}
                                           });
                        vertices.push_back({
                                                   {(float) x + 1,                 (float) y},
                                                   {1.0f,                          1.0f, 1.0f},
                                                   {rect.position.x + rect.size.x, rect.position.y}
                                           });
                    }
                }
            }
            model->setVertices(vertices);
            model->setIndices(indices);
            model->regenerateBuffers();
            tileMap->changes = false;
        }
        if(model != nullptr) {
            tileMapShader->set("SCREEN_RESOLUTION",
                               Vector2((float) Graphics::getViewportWidth(), (float) Graphics::getViewportHeight()));
            tileMapShader->set("MODEL_MATRIX", Matrix3x3::createTransform(Vector2(0, 0), Vector2(1, 1), 0));
            tileMapShader->set("Z_INDEX", 0);
            model->draw();
        }
    });
}

int main() {


    //Initailize Graphics

    Graphics::init();

    Window window;
    window.setTitle("Pomegranate Engine");
    window.setSize(800, 600);
    window.open();

    tileMapShader = new Shader("assets/shaders/tilemap/vertex.glsl", "assets/shaders/tilemap/fragment.glsl");

    //Define events

    const event_id EVENT_RENDER = Event::getEventId("render");
    const event_id EVENT_UPDATE = Event::getEventId("update");

    //Create groups

    Group world = Group("world");

    Group actors = Group("actors");

    //Create entities

    Entity map;

    auto* tileSet = map.addComponent<TileSet>(TILE_SET);
    tileSet->addTile(Tile{Rect(0,0,8,8), Vector2(0,0)});

    auto* tileMap = map.addComponent<TileMap>(TILE_MAP);
    tileMap->resize(10,10);
    tileMap->setTile(1,0,1);

    world.addEntity(&map);

    Event::on(EVENT_RENDER, tileMapRender);

    while(window.isOpen())
    {
        window.pollEvents();

        //Update
        Event::call(EVENT_UPDATE, nullptr);

        window.draw.begin();
        window.draw.clear();
        Event::call(EVENT_RENDER, nullptr);
        window.draw.end();
    }

    return 0;
}