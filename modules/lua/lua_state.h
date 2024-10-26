#ifndef POMEGRANATEENGINE_LUA_STATE_H
#define POMEGRANATEENGINE_LUA_STATE_H
#include <string>
#include "external/luajit/src/lua.hpp"
#include <core/core.h>


struct LuaTable : Reflectable {
    LuaTable() = default;
    LuaTable(lua_State* L, int idx);
    template <typename T, typename... Args>
    void addProperty(const char* name, Args&&... args) {
        property<T>(name, new T(std::forward<Args>(args)...));
    }
};

class LuaState {
public:
    lua_State* _lua;
    int _args;
    std::string _source;

    LuaState();
    ~LuaState();

    void open(const std::string& source) {
        _source = source;
        luaL_dostring(_lua, source.c_str());
    }
    void open(File file) {
        Debug::AssertIf::isFalse(file.exists(), "File does not exist");
        if(!file.isOpen())
        {
            file.open();
        }
        open(file.readText());
    }

    template<typename Ret = void, typename... Args>
    Ret call(const std::string& function = "") {
        if(_args == 0)
            lua_getglobal(_lua, function.c_str());
        lua_pcall(_lua, _args, 1, 0);
        return (Ret)lua_touserdata(_lua, -1);
    }

    template<>
    int call<int>(const std::string& function) {
        if(_args == 0)
            lua_getglobal(_lua, function.c_str());
        lua_pcall(_lua, _args, 1, 0);
        return lua_tointeger(_lua, -1);
    }

    template<>
    float call<float>(const std::string& function) {
        if(_args == 0)
            lua_getglobal(_lua, function.c_str());
        lua_pcall(_lua, _args, 1, 0);
        return (float)lua_tonumber(_lua, -1);
    }
    template<>
    double call<double>(const std::string& function) {
        if(_args == 0)
            lua_getglobal(_lua, function.c_str());
        lua_pcall(_lua, _args, 1, 0);
        return lua_tonumber(_lua, -1);
    }
    template<>
    std::string call<std::string>(const std::string& function) {
        if(_args == 0)
            lua_getglobal(_lua, function.c_str());
        lua_pcall(_lua, _args, 1, 0);
        return lua_tostring(_lua, -1);
    }
    template<>
    bool call<bool>(const std::string& function) {
        if(_args == 0)
            lua_getglobal(_lua, function.c_str());
        lua_pcall(_lua, _args, 1, 0);
        return lua_toboolean(_lua, -1);
    }
    template<>
    void call<void>(const std::string& function) {
        if(_args == 0)
            lua_getglobal(_lua, function.c_str());
        lua_pcall(_lua, _args, 0, 0); // Call without expecting a return value
    }

    template <typename T>
    void arg(T arg);

    template<>
    void arg(int arg) {
        lua_pushinteger(_lua, arg);
        _args++;
    }

    template<>
    void arg(float arg) {
        lua_pushnumber(_lua, arg);
        _args++;
    }

    template<>
    void arg(double arg) {
        lua_pushnumber(_lua, arg);
        _args++;
    }

    template<>
    void arg(const std::string& arg) {
        lua_pushstring(_lua, arg.c_str());
        _args++;
    }

    template<>
    void arg(const char* arg) {
        lua_pushstring(_lua, arg);
        _args++;
    }

    template<>
    void arg(bool arg) {
        lua_pushboolean(_lua, arg);
        _args++;
    }

    template<typename... Args>
    void args(const std::string& function, Args&&... args) {
        lua_getglobal(_lua, function.c_str());
        (arg(std::forward<Args>(args)), ...);
    }

    template<typename Function>
    void function(const std::string& name, Function function) {
        lua_pushcfunction(_lua, function);
        lua_setglobal(_lua, name.c_str());
    }

    void nameSpace(const std::string& name);
};


#endif //POMEGRANATEENGINE_LUA_STATE_H
