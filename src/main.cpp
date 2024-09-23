#include <utility>
#include <core/core.h>
#include <ecs/ecs.h>
#include <events/events.h>
#include <ecs/extensions/common/common.h>
#include <graphics/vulkan/graphics.h>
#include <math/geometry/geometry.h>
#include "lua/lua_script.h"
#include "lua/debug.h"

//Testing lua wrapper stuff
namespace LuaWrapper {
    std::unordered_map<std::string, ComponentID> luaComponents;

    struct LuaTable : Reflectable {
        LuaTable() = default;
        LuaTable(lua_State* L, int idx)
        {
            if (lua_istable(L,idx)) {
                lua_pushnil(L);
                while (lua_next(L, idx) != 0) {
                    const char* key = lua_tostring(L, -2);
                    if (lua_isnumber(L, -1)) {
                        addProperty<double>(key, lua_tonumber(L, -1));
                    } else if (lua_isstring(L, -1)) {
                        addProperty<std::string>(key, lua_tostring(L, -1));
                    } else if (lua_isboolean(L, -1)) {
                        addProperty<bool>(key, lua_toboolean(L, -1));
                    } else if (lua_istable(L, -1)) {
                        LuaTable* table = new LuaTable(L, lua_gettop(L));
                        property(key, table);
                    }
                    lua_pop(L, 1);
                }
            }
        }
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
            } else if(ref->type(key) == typeid(LuaTable).hash_code()) {
                LuaTable* table = &ref->get<LuaTable>(key);
                LuaReflectable* luaReflectable = (LuaReflectable*)lua_newuserdata(L,sizeof(LuaReflectable));
                new (luaReflectable) LuaReflectable();
                luaReflectable->reflectable = table;

                luaL_getmetatable(L,"Reflectable");
                lua_setmetatable(L,-2);
                return 1;
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
            Component::create<LuaTable>(component);
            luaComponents[component] = ECS::component_names[component];
        }
        if(luaComponents.find(component) != luaComponents.end()) {
            new(entity->add(component)) LuaTable(L,3);
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

    int Entity_newIndex(lua_State* L)
    {
        Entity* entity = (Entity*)lua_touserdata(L,1);
        const char* key = lua_tostring(L,2);

        if(ECS::component_names.contains(key)) {
            if(ECS::component_names[key] == luaComponents[key]) {
                new(entity->add(key)) LuaTable(L,3);
            }
            else {
                entity->add(key);
            }
        }
        else {
            //Add a new component
            Component::create<LuaTable>(key);
            luaComponents[key] = ECS::component_names[key];
            new(entity->add(key)) LuaTable(L,3);
        }

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

    int ECS_foreach(lua_State* L) {
        const char* component = lua_tostring(L,1);
        lua_pushvalue(L, 2); // Push the callback function
        int ref = luaL_ref(L, LUA_REGISTRYINDEX); // Store the reference to the callback in the registry

        ECS::each(component, [L, ref](void* table, Entity& entity) {
            lua_rawgeti(L, LUA_REGISTRYINDEX, ref); // Push the callback function to the stack
            LuaReflectable* luaReflectable = (LuaReflectable*)lua_newuserdata(L,sizeof(LuaReflectable));
            new (luaReflectable) LuaReflectable();
            luaReflectable->reflectable = (Reflectable*)table;

            luaL_getmetatable(L,"Reflectable");
            lua_setmetatable(L,-2);

            if (lua_pcall(L, 1, 0, 0) != LUA_OK) { // Handle Lua errors properly
                //Get the error message
                Debug::Log::error(lua_tostring(L, -1));
            }
        });
        return 0;
    }

    int Event_on(lua_State* L) {
        const char* event = lua_tostring(L,1);
        lua_pushvalue(L, 2); // Push the callback function
        int ref = luaL_ref(L, LUA_REGISTRYINDEX); // Store the reference to the callback in the registry

        Event::on(event, [L, ref]() {
            lua_rawgeti(L, LUA_REGISTRYINDEX, ref); // Push the callback function to the stack
            if (lua_pcall(L, 0, 0, 0) != LUA_OK) { // Handle Lua errors properly
                //Get the error message
                Debug::Log::error(lua_tostring(L, -1));
            }
        });
        return 0;
    }

    int Event_call(lua_State* L) {
        const char* event = lua_tostring(L,1);
        Event::call(event);
        return 0;
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

        lua_pushcfunction(L,Entity_newIndex);
        lua_setfield(L,-2,"__newindex");

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

    void registerEvents(lua_State* L) {
        lua_newtable(L);
        lua_pushvalue(L, -1);

        lua_setglobal(L, "Event");

        lua_pushcfunction(L,Event_on);
        lua_setfield(L,-2,"on");

        lua_pushcfunction(L,Event_call);
        lua_setfield(L,-2,"call");
    }

    void registerECS(lua_State* L) {
        lua_newtable(L);
        lua_pushvalue(L, -1);

        lua_setglobal(L, "ECS");

        lua_pushcfunction(L,ECS_foreach);
        lua_setfield(L,-2,"foreach");
    }

    void registerFunctions(lua_State* L) {

        //Entity
        registerEntity(L);

        //Reflectable
        registerReflectable(L);

        //Events
        registerEvents(L);

        //ECS
        registerECS(L);
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

    LuaScript script = LuaScript(File("assets/scripts/main.lua"));
    LuaDebug::registerFunctions(script);
    script.run();

#endif
}