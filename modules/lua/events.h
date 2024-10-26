#ifndef POMEGRANATEENGINE_EVENTS_H
#define POMEGRANATEENGINE_EVENTS_H
#include "events/events.h"
#include "lua_state.h"

namespace LuaEvents {
    int on(lua_State *L);
    int call(lua_State *L);
    void registerFunctions(LuaState& script);
}


#endif //POMEGRANATEENGINE_EVENTS_H
