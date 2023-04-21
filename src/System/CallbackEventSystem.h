#pragma once

#include "../ECS/ECS.h"

#include "../EventBus/EventBus.h"
#include "../Events/CallbackEvent.h"

class CallbackEventSystem: public System {
public:
    CallbackEventSystem() {}

    void SubscribeToEvent(std::unique_ptr<EventBus>& eventBus, std::string eventName, std::function<void ()> func) {
        if (eventCallbacks.find(eventName) != eventCallbacks.end()) {
            eventCallbacks[eventName].emplace_back(func);
        } else {
            std::vector<std::function<void()>> callbacks;
            callbacks.emplace_back(func);
            eventCallbacks.emplace(eventName, callbacks);
        }
        eventBus->SubscribeToEvent<CallbackEvent>(this, &CallbackEventSystem::OnEmit);
    }

    void OnEmit(CallbackEvent& event) {
        if (eventCallbacks.find(event.funcName) != eventCallbacks.end()) {
            Logger::LogT("Emit func: " + event.funcName);
            for (auto& callback : eventCallbacks[event.funcName]) {
                callback();
            }
        } else {
            Logger::Err("No event called " + event.funcName);
        }
    }

    void Update() {

    }

private:
    std::map<std::string, std::vector<std::function<void()>>> eventCallbacks;
};