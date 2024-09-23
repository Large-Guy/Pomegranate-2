#ifndef POMEGRANATEENGINE_DEBUG_H
#define POMEGRANATEENGINE_DEBUG_H
#include "lua_script.h"
#include "core/core.h"

namespace LuaDebug {
    namespace LuaLog {
        int info(lua_State *L);
        int pass(lua_State *L);
        int fail(lua_State *L);
        int warn(lua_State *L);
        int error(lua_State *L);
    }
    void registerFunctions(LuaScript& script);
}

#endif //POMEGRANATEENGINE_DEBUG_H
