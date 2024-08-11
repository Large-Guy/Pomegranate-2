#ifndef POMEGRANATEENGINE_EVENT_MANAGER_H
#define POMEGRANATEENGINE_EVENT_MANAGER_H
#include "event_typedefs.h"
#include <vector>
#include <unordered_map>
#include <functional>
#include <string>
#include <any>
#include <memory>

class EventFunctionBase {
public:
    virtual ~EventFunctionBase() = default;
};

template <typename... Args>
class EventFunctionImpl : public EventFunctionBase {
private:
    std::function<void(Args...)> _function;
public:
    explicit EventFunctionImpl(const std::function<void(Args...)>& function) : _function(function) {}
    EventFunctionImpl(const EventFunctionImpl& other) : _function(other._function) {}
    EventFunctionImpl& operator=(const EventFunctionImpl& other) {
        _function = other._function;
        return *this;
    }

    void call(Args... args) {
        _function(args...);
    }
};

class EventFunction {
private:
    std::shared_ptr<EventFunctionBase> _function;
public:
    template<typename... Args>
    EventFunction(std::function<void(Args...)> f) : _function(std::make_unique<EventFunctionImpl<Args...>>(f)) {}
    EventFunction(const EventFunction& other) : _function(std::move(other._function)) {}


    template<typename... Args> void call(Args... args) {
        static_cast<EventFunctionImpl<Args...>*>(_function.get())->call(args...);
    }
};

class Event {
private:
    static std::unordered_map<event_id, std::vector<EventFunction>> _events;
    static std::unordered_map<std::string, event_id> _eventIndex;
    static event_id _eventCounter;

    static event_id createEvent();
public:
    static void on(event_id id, EventFunction callback);
    static void on(const std::string& name, EventFunction callback);
    template <typename... Args> static void call(event_id id, Args... args) {
        for(auto& f : _events[id]) {
            f.call(args...);
        }
    }
    template <typename... Args> static void call(const std::string& name, Args... args) {
        event_id id = getEventId(name);
        call(id,args...);
    };
    static event_id getEventId(const std::string& name);
};


#endif //POMEGRANATEENGINE_EVENT_MANAGER_H
