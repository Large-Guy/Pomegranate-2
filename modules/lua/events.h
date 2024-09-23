#ifndef POMEGRANATEENGINE_EVENTS_H
#define POMEGRANATEENGINE_EVENTS_H
#include "events/events.h"
#include "lua_script.h"

namespace LuaEvents {
    int on(lua_State *L);
    int call(lua_State *L);
    void registerFunctions(LuaScript& script);
}


#endif //POMEGRANATEENGINE_EVENTS_H
