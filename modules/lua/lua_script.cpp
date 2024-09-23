#include "lua_script.h"

LuaTable::LuaTable(lua_State *L, int idx) {
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

lua_State* LuaScript::getLuaState() {
    return _lua;
}

std::string LuaScript::getSource() {
    return _source;
}

LuaScript::LuaScript() {
    _lua = luaL_newstate();
    luaL_openlibs(_lua);
}

LuaScript::LuaScript(File file) {
    _lua = luaL_newstate();
    luaL_openlibs(_lua);
    file.open();
    _source = file.readText();
    file.close();
}

LuaScript::LuaScript(const std::string& source) {
    _lua = luaL_newstate();
    luaL_openlibs(_lua);
    _source = source;
}

LuaScript::~LuaScript() {
    lua_close(_lua);
}

void LuaScript::run() {
    if (luaL_dostring(_lua, _source.c_str())) {
        const char* message = lua_tostring(_lua, -1);
        Debug::Log::error(message);
    }
}

void LuaScript::call(const std::string& function) {
    lua_getglobal(_lua, function.c_str());
    lua_pcall(_lua, _args, 0, 0);
}

void LuaScript::arg(int arg) {
    lua_pushinteger(_lua, arg);
    _args++;
}

void LuaScript::arg(float arg) {
    lua_pushnumber(_lua, arg);
    _args++;
}

void LuaScript::arg(const std::string& arg) {
    lua_pushstring(_lua, arg.c_str());
    _args++;
}

void LuaScript::arg(bool arg) {
    lua_pushboolean(_lua, arg);
    _args++;
}