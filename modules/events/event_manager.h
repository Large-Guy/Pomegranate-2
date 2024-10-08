#ifndef POMEGRANATE_ENGINE_EVENTS_EVENT_MANAGER_H
#define POMEGRANATE_ENGINE_EVENTS_EVENT_MANAGER_H
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
    EventFunction(std::function<void(Args...)> f) : _function(std::make_shared<EventFunctionImpl<Args...>>(f)) {}
    EventFunction(const EventFunction& other) : _function(std::move(other._function)) {}


    template<typename... Args> void call(Args... args) {
        static_cast<EventFunctionImpl<Args...>*>(_function.get())->call(args...);
    }
};

class Event {
private:
    static std::unordered_map<EventID, std::vector<EventFunction>> _events;
    static std::unordered_map<std::string, EventID> _eventIndex;
    static EventID _eventCounter;

    static EventID createEvent();
public:
    static void on(EventID id, EventFunction callback);
    static void on(const std::string& name, EventFunction callback);
    template <typename Func>static void on(EventID id, Func callback)
    {
        Event::_events[id].push_back(std::function(callback));
    }
    template <typename Func>static void on(const std::string& name, Func callback)
    {
        EventID id = Event::getEventId(name);
        Event::_events[id].push_back(std::function(callback));
    }
    template <typename... Args> static void call(EventID id, Args... args) {
        for(auto& f : _events[id]) {
            f.call(args...);
        }
    }
    template <typename... Args> static void call(const std::string& name, Args... args) {
        EventID id = getEventId(name);
        call(id,args...);
    };
    static EventID getEventId(const std::string& name);
};


#endif //POMEGRANATE_ENGINE_EVENT_MANAGER_H
