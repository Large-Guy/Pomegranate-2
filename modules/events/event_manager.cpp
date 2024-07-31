#include "event_manager.h"

std::unordered_map<event_id, std::vector<std::function<void(void*)>>> Event::_events;
std::unordered_map<std::string, event_id> Event::_eventIndex;
event_id Event::_eventCounter = 0;

void Event::on(event_id id, const std::function<void(void*)>& callback) {
    Event::_events[id].push_back(callback);
}

void Event::on(const std::string& name, const std::function<void(void*)>& callback) {
    event_id id = Event::getEventId(name);
    Event::_events[id].push_back(callback);
}

void Event::call(event_id id, void* data) {
    for(auto& callback : Event::_events[id]) {
        callback(data);
    }
}

void Event::call(const std::string& name, void* data) {
    event_id id = Event::getEventId(name);
    Event::call(id,data);
}

event_id Event::createEvent() {
    return Event::_eventCounter++;
}

event_id Event::getEventId(const std::string &name) {
    if(Event::_eventIndex.count(name)) {
        return Event::_eventIndex[name];
    }
    else
    {
        event_id id = Event::createEvent();
        Event::_eventIndex[name] = id;
        return id;
    }
}
