#ifndef POMEGRANATE_ENGINE_EVENTS_EVENT_MANAGER_H
#define POMEGRANATE_ENGINE_EVENTS_EVENT_MANAGER_H
#include "event_typedefs.h"
#include <vector>
#include <unordered_map>
#include <functional>
#include <string>
#include <any>
#include <memory>
#include "core/function.h"

class Event {
private:
    static std::unordered_map<EventID, std::vector<Function>> _events;
    static std::unordered_map<std::string, EventID> _eventIndex;
    static EventID _eventCounter;

    static EventID createEvent();
public:
    static void on(EventID id, Function callback);
    static void on(const std::string& name, Function callback);
    template <typename... Args> static void emit(EventID id, Args... args) {
        for(auto& f : _events[id]) {
            f.call<void>(args...);
        }
    }
    template <typename... Args> static void emit(const std::string& name, Args... args) {
        EventID id = getEventId(name);
        emit(id, args...);
    };
    static EventID getEventId(const std::string& name);
};


#endif //POMEGRANATE_ENGINE_EVENT_MANAGER_H
