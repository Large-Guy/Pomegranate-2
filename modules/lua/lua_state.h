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

enum LuaType {
    LUA_TYPE_NIL = LUA_TNIL,
    LUA_TYPE_NUMBER = LUA_TNUMBER,
    LUA_TYPE_STRING = LUA_TSTRING,
    LUA_TYPE_TABLE = LUA_TTABLE,
    LUA_TYPE_FUNCTION = LUA_TFUNCTION,
    LUA_TYPE_USERDATA = LUA_TUSERDATA,
    LUA_TYPE_THREAD = LUA_TTHREAD,
    LUA_TYPE_BOOLEAN = LUA_TBOOLEAN,
    LUA_TYPE_LIGHTUSERDATA = LUA_TLIGHTUSERDATA,
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
        int result = luaL_dostring(_lua, source.c_str());
        if (result != LUA_OK) {
            Debug::Log::error(lua_tostring(_lua, -1));
            lua_pop(_lua, 1);
        }
    }
    void open(File file) {
        Debug::AssertIf::isFalse(file.exists(), "File does not exist");
        if(!file.isOpen())
        {
            file.open();
        }
        open(file.readText());
    }

    bool global(const std::string& name) {
        lua_getglobal(_lua, name.c_str());
        if (lua_isnil(_lua, -1)) {
            lua_pop(_lua, 1);
            return false;
        }
    }

    LuaType type() {
        return (LuaType)lua_type(_lua, lua_gettop(_lua));
    }

    template<typename... Args>
    void args(Args&&... args) {
        _args = 0;
        (arg(std::forward<Args>(args)), ...);
    }

    void call(int results = 0) {
        int res = lua_pcall(_lua, _args, results, 0);
        if (res != LUA_OK) {
            Debug::Log::error(lua_tostring(_lua, -1));
            lua_pop(_lua, 1);
        }
    }

    int asInt() {
        int result = lua_tointeger(_lua, lua_gettop(_lua));
        lua_pop(_lua, 1);
        return result;
    }

    float asFloat() {
        float result = lua_tonumber(_lua, lua_gettop(_lua));
        lua_pop(_lua, 1);
        return result;
    }

    double asDouble() {
        double result = lua_tonumber(_lua, lua_gettop(_lua));
        lua_pop(_lua, 1);
        return result;
    }

    std::string asString() {
        std::string result = lua_tostring(_lua, lua_gettop(_lua));
        lua_pop(_lua, 1);
        return result;
    }

    bool asBool() {
        bool result = lua_toboolean(_lua, lua_gettop(_lua));
        lua_pop(_lua, 1);
        return result;
    }

    LuaTable asTable() {
        LuaTable table = LuaTable(_lua, lua_gettop(_lua));
        lua_pop(_lua, 1);
        return table;
    }

    void arg(int arg) {
        lua_pushinteger(_lua, arg);
        _args++;
    }

    void arg(float arg) {
        lua_pushnumber(_lua, arg);
        _args++;
    }

    void arg(double arg) {
        lua_pushnumber(_lua, arg);
        _args++;
    }

    void arg(const std::string& arg) {
        lua_pushstring(_lua, arg.c_str());
        _args++;
    }

    void arg(const char* arg) {
        lua_pushstring(_lua, arg);
        _args++;
    }

    void arg(bool arg) {
        lua_pushboolean(_lua, arg);
        _args++;
    }

    template<typename Function>
    void function(const std::string& name, Function function) {
        lua_pushcfunction(_lua, function);
        lua_setglobal(_lua, name.c_str());
    }

    void nameSpace(const std::string& name);
};


#endif //POMEGRANATEENGINE_LUA_STATE_H
