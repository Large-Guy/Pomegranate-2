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

class QuestionFunctionBase {
public:
    virtual ~QuestionFunctionBase() = default;
};

template <typename ret,typename... Args>
class QuestionFunctionImpl : public QuestionFunctionBase {
private:
    std::function<ret(Args...)> _function;
public:
    explicit QuestionFunctionImpl(const std::function<ret(Args...)>& function) : _function(function) {}
    QuestionFunctionImpl(const QuestionFunctionImpl& other) : _function(other._function) {}
    QuestionFunctionImpl& operator=(const QuestionFunctionImpl& other) {
        _function = other._function;
        return *this;
    }

    ret call(Args... args) {
        return _function(args...);
    }
};

class QuestionFunction {
private:
    std::shared_ptr<QuestionFunctionBase> _function;
public:
    template<typename Ret,typename... Args>
    QuestionFunction(std::function<Ret(Args...)> f) : _function(std::make_shared<QuestionFunctionImpl<Ret,Args...>>(f)) {}
    QuestionFunction(const QuestionFunction& other) : _function(std::move(other._function)) {}


    template<typename Ret, typename... Args> Ret call(Args... args) {
        return static_cast<QuestionFunctionImpl<Ret,Args...>*>(_function.get())->call(args...);
    }
};

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
    static std::unordered_map<QuestionID, std::vector<QuestionFunction>> _events;
    static std::unordered_map<std::string, QuestionID> _eventIndex;
    static QuestionID _eventCounter;

    static QuestionID createQuestion();
public:
    static void answer(EventID id, const QuestionFunction& callback);
    static void answer(const std::string& name, const QuestionFunction& callback);
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
