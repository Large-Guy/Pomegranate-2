#ifndef POMEGRANATEENGINE_LUA_STATE_H
#define POMEGRANATEENGINE_LUA_STATE_H
#include <string>
#include "external/luajit/src/lua.hpp"
#include <core/core.h>
#include <stack>


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

struct LuaFunction {
    lua_CFunction _function;
};

class LuaState {
public:
    lua_State* _lua;
    int _args;
    std::string _source;
    std::stack<std::string> _tableStack;

    LuaState();
    ~LuaState();

    void open(const std::string& source);
    void open(File file);

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

    void arg(int arg);
    void arg(float arg);
    void arg(double arg);
    void arg(const std::string& arg);
    void arg(const char* arg);
    void arg(bool arg);

    void function(const std::string& name, lua_CFunction function, bool global = false);

    void beginClass(const std::string& name);
    void endClass();

    void beginNamespace(const std::string& name);
    void endNamespace();
};

class LuaModule {
public:
    virtual void init(LuaState& script) = 0;
};

#endif //POMEGRANATEENGINE_LUA_STATE_H
