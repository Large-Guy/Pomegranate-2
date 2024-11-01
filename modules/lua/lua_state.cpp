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

void LuaState::open(const std::string &source) {
    _source = source;
    int result = luaL_dostring(_lua, source.c_str());
    if (result != LUA_OK) {
        Debug::Log::error(lua_tostring(_lua, -1));
        lua_pop(_lua, 1);
    }
}

void LuaState::open(File file) {
    Debug::AssertIf::isFalse(file.exists(), "File does not exist");
    if(!file.isOpen())
    {
        file.open();
    }
    open(file.readText());
}

void LuaState::arg(int arg) {
    lua_pushinteger(_lua, arg);
    _args++;
}

void LuaState::arg(float arg) {
    lua_pushnumber(_lua, arg);
    _args++;
}

void LuaState::arg(double arg) {
    lua_pushnumber(_lua, arg);
    _args++;
}

void LuaState::arg(const std::string& arg) {
    lua_pushstring(_lua, arg.c_str());
    _args++;
}

void LuaState::arg(const char* arg) {
    lua_pushstring(_lua, arg);
    _args++;
}

void LuaState::arg(bool arg) {
    lua_pushboolean(_lua, arg);
    _args++;
}

void LuaState::function(const std::string &name, lua_CFunction function, bool global) {
    if((_tableStack.empty() && global) || global) {
        lua_pushcfunction(_lua, function);
        lua_setglobal(_lua, name.c_str());
    } else {
        lua_pushcfunction(_lua, function);
        lua_setfield(_lua, -2, name.c_str());
    }
}

void LuaState::beginClass(const std::string &name) {
    //Create a metatable
    luaL_newmetatable(_lua, name.c_str());
    //Push the metatable to the stack
    lua_pushvalue(_lua, -1);
}

void LuaState::endClass() {
    //Pop the metatable
    lua_pop(_lua, 1);
}

void LuaState::beginNamespace(const std::string &name) {
    //Make a table namespace, but if it's not the first one, push it to the stack
    lua_newtable(_lua);
    _tableStack.push(name);
}

void LuaState::endNamespace() {
    //Pop the table namespace from the stack and name it
    std::string name = _tableStack.top();
    _tableStack.pop();

    //If the stack is empty, set the table as a global
    if(_tableStack.empty()) {
        lua_setglobal(_lua, name.c_str());
    } else {
        //Otherwise, set it as a field of the table on top of the stack
        lua_setfield(_lua, -2, name.c_str());
    }
}