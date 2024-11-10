#ifndef POMEGRANATEENGINE_QUESTION_MANAGER_H
#define POMEGRANATEENGINE_QUESTION_MANAGER_H
#include "event_typedefs.h"
#include <vector>
#include <unordered_map>
#include <functional>
#include <string>
#include <any>
#include <memory>
#include <core/core.h>

namespace Resolver {

    template<typename Ret>
    class Sum {
    private:
        Ret _sum;
    public:
        Sum() : _sum(0) {}

        void operator()(Ret value) {
            _sum += value;
        }

        Ret operator()() {
            return _sum;
        }
    };

    template<typename Ret>
    class Product {
    private:
        Ret _product;
    public:
        Product() : _product(1) {}

        void operator()(Ret value) {
            _product *= value;
        }

        Ret operator()() {
            return _product;
        }
    };

    template<typename Ret>
    class Greatest {
    private:
        Ret _greatest;
    public:
        Greatest() : _greatest(0) {}

        void operator()(Ret value) {
            if (value > _greatest) {
                _greatest = value;
            }
        }

        Ret operator()() {
            return _greatest;
        }
    };

    template<typename Ret>
    class Least {
    private:
        Ret _least;
    public:
        Least() : _least(0) {}

        void operator()(Ret value) {
            if (value < _least) {
                _least = value;
            }
        }

        Ret operator()() {
            return _least;
        }
    };

    template<typename Ret>
    class Array {
    private:
        List<Ret> _values;
    public:
        Array() : _values() {}

        void operator()(Ret value) {
            _values.add(value);
        }

        List<Ret> operator()() {
            return _values;
        }
    };

    template<typename Ret>
    class Or {
    private:
        Ret _value;
    public:
        Or() : _value(false) {}

        void operator()(Ret value) {
            if (value) {
                _value = value;
            }
        }

        Ret operator()() {
            return _value;
        }
    };

    template<typename Ret>
    class And {
    private:
        Ret _value;
    public:
        And() : _value(true) {}

        void operator()(Ret value) {
            if (!value) {
                _value = value;
            }
        }

        Ret operator()() {
            return _value;
        }
    };
}

class Question {
private:
    static std::unordered_map<QuestionID, std::vector<Function>> _events;
    static std::unordered_map<std::string, QuestionID> _eventIndex;
    static QuestionID _eventCounter;

    static QuestionID createQuestion();
public:
    static void answer(EventID id, const Function& callback);
    static void answer(const std::string& name, const Function& callback);
    template <typename Ret, template<typename T> class Resolver,
            typename RetOverride = Ret,typename... Args> static RetOverride ask(EventID id, Args... args) {
        Resolver<Ret> answer;
        for(auto& f : _events[id]) {
            answer(f.call<Ret>(args...));
        }

        return answer();
    }
    template <typename Ret,template<typename T> class Resolver, typename RetOverride = Ret,typename... Args> static RetOverride ask(const std::string& name, Args... args) {
        return ask<Ret,Resolver>(getQuestionId(name),args...);
    }
    static QuestionID getQuestionId(const std::string& name);
};


#endif //POMEGRANATEENGINE_QUESTION_MANAGER_H
