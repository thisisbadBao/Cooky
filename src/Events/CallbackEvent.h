#pragma once

#include "../ECS/ECS.h"
#include "../EventBus/Event.h"

class CallbackEvent: public Event {
public:
    std::string funcName;

    CallbackEvent(std::string funcName) : funcName(funcName) {}
};