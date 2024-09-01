#ifndef POMEGRANATE_ENGINE_CORE_DEBUG_H
#define POMEGRANATE_ENGINE_CORE_DEBUG_H
#include "string.h"

class Debug {
public:
    class Log {
    public:
        static void info(String message);
        static void warn(String message);
        static void error(String message);
        static void pass(String message);
        static void fail(String message);
    };
};


#endif //POMEGRANATEENGINE_DEBUG_H
