#ifndef POMEGRANATEENGINE_LUA_SCRIPT_H
#define POMEGRANATEENGINE_LUA_SCRIPT_H
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

class LuaScript {
private:
    lua_State* _lua;
    std::string _source;
    int _args;
public:
    lua_State* getLuaState();
    std::string getSource();

    LuaScript();
    LuaScript(File file);
    LuaScript(const std::string& source);
    ~LuaScript();

    void run();
    void call(const std::string& function);
    void arg(int arg);
    void arg(float arg);
    void arg(const std::string& arg);
    void arg(bool arg);
};


#endif //POMEGRANATEENGINE_LUA_SCRIPT_H
