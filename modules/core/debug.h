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
    class AssertIf {
    public:
        static inline void isTrue(bool condition, const String& message) {
            if (condition) {
                Log::fail(message);
                throw std::runtime_error(message.data());
            }
        }
        static inline void isFalse(bool condition, const String& message) {
            if (!condition) {
                Log::fail(message);
                throw std::runtime_error(message.data());
            }
        }
        static inline void isNull(void* ptr, const String& message) {
            if (ptr == nullptr) {
                Log::fail(message);
                throw std::runtime_error(message.data());
            }
        }
        static inline void isNotNull(void* ptr, const String& message) {
            if (ptr != nullptr) {
                Log::fail(message);
                throw std::runtime_error(message.data());
            }
        }
        static inline void isZero(int value, const String& message) {
            if (value == 0) {
                Log::fail(message);
                throw std::runtime_error(message.data());
            }
        }
        static inline void isNotZero(int value, const String& message) {
            if (value != 0) {
                Log::fail(message);
                throw std::runtime_error(message.data());
            }
        }
        static inline void isEqual(int a, int b, const String& message) {
            if (a == b) {
                Log::fail(message);
                throw std::runtime_error(message.data());
            }
        }
        static inline void isNotEqual(int a, int b, const String& message) {
            if (a != b) {
                Log::fail(message);
                throw std::runtime_error(message.data());
            }
        }
        static inline void isLessThan(int a, int b, const String& message) {
            if (a < b) {
                Log::fail(message);
                throw std::runtime_error(message.data());
            }
        }
        static inline void isLessThanOrEqual(int a, int b, const String& message) {
            if (a <= b) {
                Log::fail(message);
                throw std::runtime_error(message.data());
            }
        }
        static inline void isGreaterThan(int a, int b, const String& message) {
            if (a > b) {
                Log::fail(message);
                throw std::runtime_error(message.data());
            }
        }
        static inline void isGreaterThanOrEqual(int a, int b, const String& message) {
            if (a >= b) {
                Log::fail(message);
                throw std::runtime_error(message.data());
            }
        }
    };
    class WarnIf {
    public:
        static inline void isTrue(bool condition, const String& message) {
            if (condition) {
                Log::warn(message);
            }
        }
        static inline void isFalse(bool condition, const String& message) {
            if (!condition) {
                Log::warn(message);
            }
        }
        static inline void isNull(void* ptr, const String& message) {
            if (ptr == nullptr) {
                Log::warn(message);
            }
        }
        static inline void isNotNull(void* ptr, const String& message) {
            if (ptr != nullptr) {
                Log::warn(message);
            }
        }
        static inline void isZero(int value, const String& message) {
            if (value == 0) {
                Log::warn(message);
            }
        }
        static inline void isNotZero(int value, const String& message) {
            if (value != 0) {
                Log::warn(message);
            }
        }
        static inline void isEqual(int a, int b, const String& message) {
            if (a == b) {
                Log::warn(message);
            }
        }
        static inline void isNotEqual(int a, int b, const String& message) {
            if (a != b) {
                Log::warn(message);
            }
        }
        static inline void isLessThan(int a, int b, const String& message) {
            if (a < b) {
                Log::warn(message);
            }
        }
        static inline void isLessThanOrEqual(int a, int b, const String& message) {
            if (a <= b) {
                Log::warn(message);
            }
        }
        static inline void isGreaterThan(int a, int b, const String& message) {
            if (a > b) {
                Log::warn(message);
            }
        }
        static inline void isGreaterThanOrEqual(int a, int b, const String& message) {
            if (a >= b) {
                Log::warn(message);
            }
        }
    };
};


#endif //POMEGRANATEENGINE_DEBUG_H
