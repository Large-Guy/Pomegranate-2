#include "lua_script.h"

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