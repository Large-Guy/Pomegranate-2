#include "lua_state.h"

LuaTable::LuaTable(lua_State *L, int idx) {
    if (lua_istable(L, idx)) {
        lua_pushnil(L);  // Start `lua_next` with `nil` to get the first key
        while (lua_next(L, idx) != 0) {
            const char* key = lua_tostring(L, -2);
            if (key == nullptr) {
                lua_pop(L, 1);  // Pop the value and skip if key is not a string
                continue;
            }
            // Handle different types
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
            lua_pop(L, 1);  // Pop the value, keeping the key for the next iteration
        }
    }
}


LuaState::LuaState() {
    _lua = luaL_newstate();
    luaL_openlibs(_lua);
    _args = 0;
}

LuaState::~LuaState() {
    lua_close(_lua);
}

void LuaState::nameSpace(const std::string &name) {
    lua_newtable(_lua);
    lua_pushvalue(_lua, -1);
    lua_setglobal(_lua, name.c_str());
}