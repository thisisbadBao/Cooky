#pragma once

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/CallbackEvent.h"

class CallbackEventSystem: public System {
public:
    CallbackEventSystem() {}

    void Init(std::unique_ptr<EventBus>& eventBus) {
        eventBus->SubscribeToEvent<CallbackEvent>(this, &CallbackEventSystem::OnEmit);
    }

    void SubscribeToEvent(std::string eventName, std::function<void ()> func) {
        if (eventCallbacks.find(eventName) != eventCallbacks.end()) {
            eventCallbacks[eventName].emplace_back(func);
        } else {
            std::vector<std::function<void()>> callbacks;
            callbacks.emplace_back(func);
            eventCallbacks.emplace(eventName, callbacks);
        }
    }

    void Reset() {
        eventCallbacks.clear();
    }

private:
    void OnEmit(CallbackEvent& event) {
        if (eventCallbacks.find(event.funcName) != eventCallbacks.end()) {
            for (auto& callback : eventCallbacks[event.funcName]) {
                callback();
            }
        }
    }

private:
    std::map<std::string, std::vector<std::function<void()>>> eventCallbacks;
};