#include "event_manager.h"

std::unordered_map<EventID, std::vector<Function>> Event::_events;
std::unordered_map<std::string, EventID> Event::_eventIndex;
EventID Event::_eventCounter = 0;

void Event::on(EventID id, Function callback) {
    Event::_events[id].push_back(callback);
}

void Event::on(const std::string& name, Function callback) {
    EventID id = Event::create(name);
    Event::_events[id].push_back(callback);
}

EventID Event::createEvent() {
    return Event::_eventCounter++;
}

EventID Event::create(const std::string &name) {
    if(Event::_eventIndex.count(name)) {
        return Event::_eventIndex[name];
    }
    else
    {
        EventID id = Event::createEvent();
        Event::_eventIndex[name] = id;
        return id;
    }
}

EventID Event::get(const std::string &name) {
    return Event::_eventIndex[name];
}