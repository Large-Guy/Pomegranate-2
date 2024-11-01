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
    lua_pushnil(L);
    return 1;
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
    lua_pushnil(L);
    return 1;
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
    auto* entity = (Entity*)lua_touserdata(L, 1);
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

    // Retrieve the entity
    Entity* entity = (Entity*)lua_touserdata(L, 1);

    // Retrieve the key
    const char* key = lua_tostring(L, 2);

    // Use luaL_getmetafield to check if the key is a method in the metatable
    if (luaL_getmetafield(L, 1, key)) {
        if (lua_isfunction(L, -1)) {
            return 1;  // Function found and pushed onto the stack
        }
        lua_pop(L, 1);  // Clean up if it's not a function
    }

    // Check if the key is "id"
    if (strcmp(key, "id") == 0) {
        lua_pushinteger(L, entity->id);
        return 1;
    }

    // Check for a component
    if (ECS::component_names.contains(key) && entity->has(key)) {
        void* data = Entity::getComponent(entity->id, key);

        // Assume data is a reflectable object
        Reflectable* reflectable = (Reflectable*)data;

        LuaReflectable* luaReflectable = (LuaReflectable*)lua_newuserdata(L, sizeof(LuaReflectable));
        new (luaReflectable) LuaReflectable();
        luaReflectable->reflectable = reflectable;

        luaL_getmetatable(L, "Reflectable");
        lua_setmetatable(L, -2);
        return 1;
    }

    Debug::Log::warn("Property not found: ", key);
    lua_pushnil(L);
    return 1;
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

    if(n >= 1) {
        new(entity) Entity(lua_tointeger(L,1));
    } else {
        new(entity) Entity(Entity::create().id);
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
    script.beginClass("Entity"); //Entity class

    script.function("__index",LuaEntity::index);
    script.function("__newindex",LuaEntity::newIndex);
    script.function("has",LuaEntity::has);
    script.function("get",LuaEntity::get);
    script.function("add",LuaEntity::add);

    script.endClass();


    script.beginNamespace("Entity"); // Entity static functions

    script.function("new",LuaEntity::newEntity);

    script.endNamespace();


    script.beginClass("Reflectable"); //Reflectables class

    script.function("__index",LuaReflection::index);
    script.function("__newindex",LuaReflection::newIndex);

    script.endClass();


    script.beginNamespace("ECS"); //ECS

    script.function("foreach",LuaECS::foreach);

    script.endNamespace();
}