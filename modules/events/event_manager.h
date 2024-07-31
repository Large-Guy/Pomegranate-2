#ifndef POMEGRANATEENGINE_EVENT_MANAGER_H
#define POMEGRANATEENGINE_EVENT_MANAGER_H
#include "event_typedefs.h"
#include <vector>
#include <unordered_map>
#include <functional>
#include <string>

class Event {
private:
    static std::unordered_map<event_id, std::vector<std::function<void(void*)>>> _events;
    static std::unordered_map<std::string, event_id> _eventIndex;
    static event_id _eventCounter;

    static event_id createEvent();
public:
    static void on(event_id id, const std::function<void(void*)>& callback);
    static void on(const std::string& name, const std::function<void(void*)>& callback);
    static void call(event_id id, void* data);
    static void call(const std::string& name, void* data);
    static event_id getEventId(const std::string& name);
};


#endif //POMEGRANATEENGINE_EVENT_MANAGER_H
