#include "ecs.h"

std::unordered_map<std::string, ComponentID> LuaECS::luaComponents = {};

int LuaECS::LuaReflection::index(lua_State* L) {
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

int LuaECS::LuaReflection::newIndex(lua_State* L) {
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

int LuaECS::LuaEntity::has(lua_State* L)
{
    Entity* entity = (Entity*)lua_touserdata(L,1);
    const char* component = lua_tostring(L,2);

    bool has = Entity::hasComponent(entity->id,component);

    lua_pushboolean(L,has);
    return 1;
}

int LuaECS::LuaEntity::get(lua_State* L)
{
    using namespace LuaReflection;

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

int LuaECS::LuaEntity::add(lua_State* L)
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

int LuaECS::LuaEntity::index(lua_State* L)
{
    using namespace LuaReflection;

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

int LuaECS::LuaEntity::newIndex(lua_State* L)
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

int LuaECS::LuaEntity::newEntity(lua_State* L)
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

int LuaECS::foreach(lua_State* L) {
    using namespace LuaReflection;

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

void LuaECS::registerFunctions(LuaState &script) {
    lua_State* L = script._lua;

    luaL_newmetatable(L, "Entity");

    lua_pushcfunction(L,LuaEntity::has);
    lua_setfield(L,-2,"has");

    lua_pushcfunction(L,LuaEntity::add);
    lua_setfield(L,-2,"add");

    lua_pushcfunction(L,LuaEntity::get);
    lua_setfield(L,-2,"get");

    lua_pushcfunction(L,LuaEntity::index);
    lua_setfield(L,-2,"__index");

    lua_pushcfunction(L,LuaEntity::newIndex);
    lua_setfield(L,-2,"__newindex");

    lua_pushcfunction(L,LuaEntity::newEntity);
    lua_setglobal(L,"Entity");

    luaL_newmetatable(L, "Reflectable");

    lua_pushcfunction(L, LuaReflection::index);
    lua_setfield(L, -2, "__index");

    lua_pushcfunction(L, LuaReflection::newIndex);
    lua_setfield(L, -2, "__newindex");

    script.nameSpace("ECS");

    lua_pushcfunction(L,LuaECS::foreach);
    lua_setfield(L,-2,"foreach");
}