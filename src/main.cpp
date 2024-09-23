#include <utility>
#include <core/core.h>
#include <ecs/ecs.h>
#include <events/events.h>
#include <ecs/extensions/common/common.h>
#include <graphics/vulkan/graphics.h>
#include <math/geometry/geometry.h>
#include "lua.hpp"

//Testing lua wrapper stuff
namespace LuaWrapper {
    std::unordered_map<std::string, ComponentID> luaComponents;

    int Debug_Log_info(lua_State* L) {
        //Get arg count
        int n = lua_gettop(L);

        //Concatenate all args
        std::string message;
        for(int i = 1; i <= n; i++) {
            message += lua_tostring(L, i);
            if(i != n) {
                message += " ";
            }
        }

        //Log message
        Debug::Log::info(message);
        return 0;
    }

    struct LuaComponent : Reflectable {
        LuaComponent() = default;
        template <typename T, typename... Args>
        void addProperty(const char* name, Args&&... args) {
            property<T>(name, new T(std::forward<Args>(args)...));
        }
    };

    struct LuaReflectable {
        Reflectable* reflectable;
    };

    int Reflectable_index(lua_State* L) {
        //Get arg count

        auto* reflectable = (LuaReflectable*)lua_touserdata(L,1);

        int n = lua_gettop(L);

        //Get the key
        const char* key = lua_tostring(L,2);

        Reflectable* ref = reflectable->reflectable;

        if(ref->has(key)) {
            if(ref->type(key) == typeid(double).hash_code()) {
                lua_pushnumber(L,ref->get<double>(key));
            } else if(ref->type(key) == typeid(float).hash_code()) {
                lua_pushnumber(L,ref->get<float>(key));
            } else if(ref->type(key) == typeid(int).hash_code()) {
                lua_pushinteger(L,ref->get<int>(key));
            } else if(ref->type(key) == typeid(bool).hash_code()) {
                lua_pushboolean(L,ref->get<bool>(key));
            } else if(ref->type(key) == typeid(std::string).hash_code()) {
                lua_pushstring(L,ref->get<std::string>(key).c_str());
            }
            return 1;
        }

        Debug::Log::error("Property not found");

        return 0;
    }

    int Reflectable_newIndex(lua_State* L) {
        //Get arg count

        auto* reflectable = (LuaReflectable*)lua_touserdata(L,1);

        int n = lua_gettop(L);

        //Get the key
        std::string key = lua_tostring(L,2);

        Reflectable* ref = reflectable->reflectable;

        if(ref->has(key)) {
            if(ref->type(key) == typeid(double).hash_code()) {
                ref->set<double>(key,lua_tonumber(L,3));
            } else if(ref->type(key) == typeid(float).hash_code()) {
                ref->set<float>(key,(float)lua_tonumber(L,3));
            } else if(ref->type(key) == typeid(int).hash_code()) {
                ref->set<int>(key,lua_tointeger(L,3));
            } else if(ref->type(key) == typeid(bool).hash_code()) {
                ref->set<bool>(key,lua_toboolean(L,3));
            } else if(ref->type(key) == typeid(std::string).hash_code()) {
                ref->set<std::string>(key, lua_tostring(L, 3));
            }
            return 1;
        }

        Debug::Log::error("Property not found");

        return 0;
    }

    int Entity_has(lua_State* L)
    {
        Entity* entity = (Entity*)lua_touserdata(L,1);
        const char* component = lua_tostring(L,2);

        bool has = Entity::hasComponent(entity->id,component);

        lua_pushboolean(L,has);
        return 1;
    }

    int Entity_get(lua_State* L)
    {
        Entity* entity = (Entity*)lua_touserdata(L,1);
        const char* component = lua_tostring(L,2);

        void* data = Entity::getComponent(entity->id,component);

        //Assume data is a reflectable object
        Reflectable* reflectable = (Reflectable*)data;

        LuaReflectable* luaReflectable = (LuaReflectable*)lua_newuserdata(L,sizeof(LuaReflectable));
        new (luaReflectable) LuaReflectable();
        luaReflectable->reflectable = reflectable;

        luaL_getmetatable(L,"Reflectable");
        lua_setmetatable(L,-2);
        return 1;
    }

    int Entity_add(lua_State* L)
    {
        Entity* entity = (Entity*)lua_touserdata(L,1);
        const char* component = lua_tostring(L,2);

        if(ECS::component_names.find(component) == ECS::component_names.end()) {
            //Component doesn't exist create it
            Component::create<LuaComponent>(component);
            luaComponents[component] = ECS::component_names[component];
        }
        if(luaComponents.find(component) != luaComponents.end()) {
            new(entity->add(component)) LuaComponent();

            if (lua_istable(L, 3)) {
                LuaComponent *luaComponent = entity->get<LuaComponent>(component);
                lua_pushnil(L);
                while (lua_next(L, 3) != 0) {
                    const char *key = lua_tostring(L, -2);
                    if (lua_isnumber(L, -1)) {
                        luaComponent->addProperty<double>(key, lua_tonumber(L, -1));
                    } else if (lua_isstring(L, -1)) {
                        luaComponent->addProperty<std::string>(key, lua_tostring(L, -1));
                    }
                    lua_pop(L, 1);
                }
            }
        }
        else {
            entity->add(component);
        }
        return 0;
    }

    int Entity_index(lua_State* L)
    {
        Entity* entity = (Entity*)lua_touserdata(L,1);
        const char* key = lua_tostring(L,2);

        // Check if the key matches a method name
        lua_getmetatable(L, 1);      // Push metatable of the entity
        lua_pushstring(L, key);      // Push the method name (key)
        lua_rawget(L, -2);           // Try to get the function from the metatable

        // If it's a function, return it
        if (lua_isfunction(L, -1)) {
            return 1;
        }

        if(strcmp(key,"id") == 0) {
            lua_pushinteger(L,entity->id);
            return 1;
        }

        if(ECS::component_names.contains(key) && entity->has(key)) {
            void* data = Entity::getComponent(entity->id,key);

            //Assume data is a reflectable object
            Reflectable* reflectable = (Reflectable*)data;

            LuaReflectable* luaReflectable = (LuaReflectable*)lua_newuserdata(L,sizeof(LuaReflectable));
            new (luaReflectable) LuaReflectable();
            luaReflectable->reflectable = reflectable;

            luaL_getmetatable(L,"Reflectable");
            lua_setmetatable(L,-2);
            return 1;
        }

        Debug::Log::warn("Property not found");


        return 0;
    }

    int Entity_new(lua_State* L)
    {
        //Get arg count
        int n = lua_gettop(L);

        Entity* entity = (Entity*)lua_newuserdata(L,sizeof(Entity));

        if(n == 1) {
            entity->id = lua_tointeger(L,1);
        } else {
            entity->id = Entity::create().id;
        }

        luaL_getmetatable(L,"Entity");
        lua_setmetatable(L,-2);

        return 1;
    }

    void registerEntity(lua_State* L) {
        luaL_newmetatable(L, "Entity");

        lua_pushcfunction(L,Entity_has);
        lua_setfield(L,-2,"has");

        lua_pushcfunction(L,Entity_add);
        lua_setfield(L,-2,"add");

        lua_pushcfunction(L,Entity_get);
        lua_setfield(L,-2,"get");

        lua_pushcfunction(L,Entity_index);
        lua_setfield(L,-2,"__index");

        lua_pushcfunction(L,Entity_new);
        lua_setglobal(L,"Entity");
    }

    void registerReflectable(lua_State* L) {
        luaL_newmetatable(L, "Reflectable");

        lua_pushcfunction(L, Reflectable_index);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, Reflectable_newIndex);
        lua_setfield(L, -2, "__newindex");
    }

    void registerFunctions(lua_State* L) {
        //Debug namespace
        lua_newtable(L);
        lua_pushvalue(L, -1);

        lua_setglobal(L, "Debug");

        //Log
        lua_newtable(L);

        lua_pushcfunction(L,Debug_Log_info);
        lua_setfield(L,-2,"info");

        lua_setfield(L,-2,"Log");

        //Entity
        registerEntity(L);

        //Reflectable
        registerReflectable(L);
    }
};

int main() {

//#define GRAPHICS

#ifdef GRAPHICS
    Graphics::enableValidationLayers = true;


//region Shader
    auto vertexFile = File("assets/graphics/shaders/shader.vert.spv");
    vertexFile.open();
    auto fragmentFile = File("assets/graphics/shaders/shader.frag.spv");
    fragmentFile.open();

    auto vertexShader = vertexFile.readBuffer();
    auto fragmentShader = fragmentFile.readBuffer();

    RenderInfo renderInfo = {
            .renderMode = RENDER_MODE_FILL,
            .cullMode = CULL_MODE_NONE,
            .topologyMode = TOPOLOGY_MODE_TRIANGLE_INDEXED
    };

    Shader shader = Shader<Vertex3D>(vertexShader, fragmentShader, renderInfo);
//endregion

//region Model
//RGB triangle model
    List<Vertex3D> vertices = {
            {{-0.5,-0.5,0.5},{0.0,0.0},{0.0,0.0,1.0},{1.0,0.0,0.0}},
            {{0.5,-0.5,0.5},{1.0,0.0},{0.0,0.0,1.0},{0.0,1.0,0.0}},
            {{-0.5,0.5,0.5},{0.0,1.0},{0.0,0.0,1.0},{1.0,1.0,0.0}},
            {{0.5,0.5,0.5},{1.0,1.0},{0.0,0.0,1.0},{0.0,0.0,1.0}}
    };

    List<uint16_t> indices = {
            0, 1, 2,
            2, 1, 3
    };

    Mesh3D mesh(vertices,indices, &shader);
//endregion

    Window window;

    window.setTitle("Pomegranate Engine - Vulkan");
    window.setSize(800, 600);
    window.show();

    double lastTime = glfwGetTime();
    double deltaTime = 0.0;

    while(window.isOpen()) {
        window.poll();

        shader.updateUniformBuffer(window._currentFrame);

        window.draw.begin();
        window.draw.clear({0.1, 0.1, 0.1, 1.0});

        window.draw.mesh(mesh);

        window.draw.end();

        double currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        Debug::Log::info("FPS: ",1.0/deltaTime);
    }

    return 0;
#else

    Extensions::Common::init();

    Entity entity = Entity::create();
    entity.add<Transform2D>();

    entity.get<Transform2D>()->position = {1.0, 2.0};
    entity.get<Transform2D>()->rotation = 3.0;
    entity.get<Transform2D>()->scale = {4.0, 5.0};

    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    LuaWrapper::registerFunctions(L);

    luaL_dofile(L, "assets/scripts/main.lua");

    //Check for errors
    if(lua_isstring(L,-1)) {
        Debug::Log::error(lua_tostring(L,-1));
    }

    lua_close(L);
#endif
}