#ifndef POMEGRANATEENGINE_ECS_H
#define POMEGRANATEENGINE_ECS_H
#include "ecs/ecs.h"
#include "lua_state.h"

namespace LuaECS {
    extern std::unordered_map<std::string, ComponentID> luaComponents;

    namespace LuaReflection {
        struct LuaReflectable {
            Reflectable* reflectable;
        };

        int index(lua_State* L);
        int newIndex(lua_State* L);
    }
    namespace LuaEntity {
        int has(lua_State* L);
        int get(lua_State* L);
        int add(lua_State* L);
        int index(lua_State* L);
        int newIndex(lua_State* L);
        int newEntity(lua_State* L);
    }

    int foreach(lua_State* L);

    void registerFunctions(LuaState& script);
}


#endif //POMEGRANATEENGINE_ECS_H
