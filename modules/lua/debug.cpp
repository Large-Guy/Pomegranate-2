#include "debug.h"

int LuaDebug::LuaLog::info(lua_State *L) {
//Get arg count
    int n = lua_gettop(L);

//Concatenate all args
    std::string message;
    for (int i = 1; i <= n; i++) {
        message += lua_tostring(L, i);
        if (i != n) {
            message += " ";
        }
    }

//Log message
    Debug::Log::info(message);
    return 0;
}

int LuaDebug::LuaLog::pass(lua_State *L) {
//Get arg count
    int n = lua_gettop(L);

//Concatenate all args
    std::string message;
    for (int i = 1; i <= n; i++) {
        message += lua_tostring(L, i);
        if (i != n) {
            message += " ";
        }
    }

//Log message
    Debug::Log::pass(message);
    return 0;
}

int LuaDebug::LuaLog::fail(lua_State *L) {
//Get arg count
    int n = lua_gettop(L);

//Concatenate all args
    std::string message;
    for (int i = 1; i <= n; i++) {
        message += lua_tostring(L, i);
        if (i != n) {
            message += " ";
        }
    }

//Log message
    Debug::Log::fail(message);
    return 0;
}

int LuaDebug::LuaLog::warn(lua_State *L) {
//Get arg count
    int n = lua_gettop(L);

//Concatenate all args
    std::string message;
    for (int i = 1; i <= n; i++) {
        message += lua_tostring(L, i);
        if (i != n) {
            message += " ";
        }
    }

//Log message
    Debug::Log::warn(message);
    return 0;
}

int LuaDebug::LuaLog::error(lua_State *L) {
//Get arg count
    int n = lua_gettop(L);

//Concatenate all args
    std::string message;
    for (int i = 1; i <= n; i++) {
        message += lua_tostring(L, i);
        if (i != n) {
            message += " ";
        }
    }

//Log message
    Debug::Log::error(message);
    return 0;
}

void LuaDebug::registerFunctions(LuaState& script) {
    lua_State* L = script._lua;

    script.beginNamespace("Debug");

    //Log
    script.beginNamespace("Log");

    lua_pushcfunction(L, LuaLog::info);
    lua_setfield(L,-2,"info");

    lua_pushcfunction(L, LuaLog::pass);
    lua_setfield(L,-2,"pass");

    lua_pushcfunction(L, LuaLog::fail);
    lua_setfield(L,-2,"fail");

    lua_pushcfunction(L, LuaLog::warn);
    lua_setfield(L,-2,"warn");

    lua_pushcfunction(L, LuaLog::error);
    lua_setfield(L,-2,"error");

    script.endNamespace();

    script.endNamespace();
}
