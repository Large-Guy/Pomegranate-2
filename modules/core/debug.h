#ifndef POMEGRANATE_ENGINE_CORE_DEBUG_H
#define POMEGRANATE_ENGINE_CORE_DEBUG_H
#include <string>
#include <iostream>

class Debug {
public:
    class Log {
    public:
        template<typename ...args>
        Log(args... messages)
        {
            std::cout << "\033[1;44m[LOG]\033[0m  ";
            // Ensure space between messages
            ((std::cout << messages << ' '), ...);
            std::cout << std::endl;
        }

        static void info(std::string message);
        static void warn(std::string message);
        static void error(std::string message);
        static void pass(std::string message);
        static void fail(std::string message);

        template<typename ...args>
        static void info(args... messages) {
            std::cout << "\033[1;44m[INFO]\033[0m  ";
            // Ensure space between messages
            ((std::cout << messages << ' '), ...);
            std::cout << std::endl;
        }

        template<typename ...args>
        static void warn(args... messages) {
            std::cout << "\033[1;43m[WARN]\033[0m  ";
            // Ensure space between messages
            ((std::cout << messages << ' '), ...);
            std::cout << std::endl;
        }

        template<typename ...args>
        static void error(args... messages) {
            std::cout << "\033[1;41m[ERROR]\033[0m ";
            // Ensure space between messages
            ((std::cout << messages << ' '), ...);
            std::cout << std::endl;
        }

        template<typename ...args>
        static void pass(args... messages) {
            std::cout << "\033[1;42m[PASS]\033[0m  ";
            // Ensure space between messages
            ((std::cout << messages << ' '), ...);
            std::cout << std::endl;
        }

        template<typename ...args>
        static void fail(args... messages) {
            std::cout << "\033[1;41m[FAIL]\033[0m  ";
            // Ensure space between messages
            ((std::cout << messages << ' '), ...);
            std::cout << std::endl;
        }
    };
    class AssertIf {
    public:
        static inline void isTrue(bool condition, const std::string& message) {
            if (condition) {
                Log::fail(message);
                throw std::runtime_error(message.data());
            }
        }
        static inline void isFalse(bool condition, const std::string& message) {
            if (!condition) {
                Log::fail(message);
                throw std::runtime_error(message.data());
            }
        }
        static inline void isNull(void* ptr, const std::string& message) {
            if (ptr == nullptr) {
                Log::fail(message);
                throw std::runtime_error(message.data());
            }
        }
        static inline void isNotNull(void* ptr, const std::string& message) {
            if (ptr != nullptr) {
                Log::fail(message);
                throw std::runtime_error(message.data());
            }
        }
        static inline void isZero(int value, const std::string& message) {
            if (value == 0) {
                Log::fail(message);
                throw std::runtime_error(message.data());
            }
        }
        static inline void isNotZero(int value, const std::string& message) {
            if (value != 0) {
                Log::fail(message);
                throw std::runtime_error(message.data());
            }
        }
        static inline void isEqual(int a, int b, const std::string& message) {
            if (a == b) {
                Log::fail(message);
                throw std::runtime_error(message.data());
            }
        }
        static inline void isNotEqual(int a, int b, const std::string& message) {
            if (a != b) {
                Log::fail(message);
                throw std::runtime_error(message.data());
            }
        }
        static inline void isLessThan(int a, int b, const std::string& message) {
            if (a < b) {
                Log::fail(message);
                throw std::runtime_error(message.data());
            }
        }
        static inline void isLessThanOrEqual(int a, int b, const std::string& message) {
            if (a <= b) {
                Log::fail(message);
                throw std::runtime_error(message.data());
            }
        }
        static inline void isGreaterThan(int a, int b, const std::string& message) {
            if (a > b) {
                Log::fail(message);
                throw std::runtime_error(message.data());
            }
        }
        static inline void isGreaterThanOrEqual(int a, int b, const std::string& message) {
            if (a >= b) {
                Log::fail(message);
                throw std::runtime_error(message.data());
            }
        }
    };
    class WarnIf {
    public:
        static inline void isTrue(bool condition, const std::string& message) {
            if (condition) {
                Log::warn(message);
            }
        }
        static inline void isFalse(bool condition, const std::string& message) {
            if (!condition) {
                Log::warn(message);
            }
        }
        static inline void isNull(void* ptr, const std::string& message) {
            if (ptr == nullptr) {
                Log::warn(message);
            }
        }
        static inline void isNotNull(void* ptr, const std::string& message) {
            if (ptr != nullptr) {
                Log::warn(message);
            }
        }
        static inline void isZero(int value, const std::string& message) {
            if (value == 0) {
                Log::warn(message);
            }
        }
        static inline void isNotZero(int value, const std::string& message) {
            if (value != 0) {
                Log::warn(message);
            }
        }
        static inline void isEqual(int a, int b, const std::string& message) {
            if (a == b) {
                Log::warn(message);
            }
        }
        static inline void isNotEqual(int a, int b, const std::string& message) {
            if (a != b) {
                Log::warn(message);
            }
        }
        static inline void isLessThan(int a, int b, const std::string& message) {
            if (a < b) {
                Log::warn(message);
            }
        }
        static inline void isLessThanOrEqual(int a, int b, const std::string& message) {
            if (a <= b) {
                Log::warn(message);
            }
        }
        static inline void isGreaterThan(int a, int b, const std::string& message) {
            if (a > b) {
                Log::warn(message);
            }
        }
        static inline void isGreaterThanOrEqual(int a, int b, const std::string& message) {
            if (a >= b) {
                Log::warn(message);
            }
        }
    };
};


#endif //POMEGRANATEENGINE_DEBUG_H
