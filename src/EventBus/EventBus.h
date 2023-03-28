#ifndef EVENTBUS_H
#define EVENTBUS_H

#include "SDL2/SDL.h"
#include <map>
#include <typeindex>
#include <list>

#include "../Logger/Logger.h"
#include "Event.h"
class IEventCallback {
private:
    virtual void Call(Event &e) = 0;

public:
    virtual ~IEventCallback() = default;

    void Execute(Event& e) {
        Call(e);
    }
};

template <typename TOwner, typename TEvent>
class EventCallBack : public IEventCallback {
private:
    typedef void (TOwner::*CallbackFunction)(TEvent&);

    TOwner *ownerInstance;
    CallbackFunction callbackFunction;

    virtual void Call(Event &e) override {
        std::invoke(callbackFunction, ownerInstance, static_cast<TEvent&>(e));
    }

public:
    EventCallBack(TOwner* ownerInstance, CallbackFunction callbackFunction) {
        this->ownerInstance = ownerInstance;
        this->callbackFunction = callbackFunction;
    }

    // virtual ~EventCallBack() override = defalut;
};

typedef std::list<std::unique_ptr<IEventCallback>> HandlerList;

class EventBus {
private:
    std::map<std::type_index, std::unique_ptr<HandlerList>> subscribers;

public:
    EventBus() {
        Logger::LogD("EventBus constructor called!");
    }

    ~EventBus() {
        Logger::LogD("EventBus destructor called!");
    }

    // A listener subscribes to an event
    template <typename TEvent, typename TOwner>
    void SubscribeToEvent(TOwner* ownerInstance, void (TOwner::*CallbackFunction)(TEvent&)) {
        if (!subscribers[typeid(TEvent)].get()) {
            subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
        }
        auto subscriber = std::make_unique<EventCallBack<TOwner, TEvent>>(ownerInstance, CallbackFunction);
        subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
    }
    // TODO: UnsubscribeEvent

    // When something emits, excute the callback function immediately
    template <typename TEvent, typename ...TArgs>
    void EmitEvent(TArgs&& ...args) {
        auto handlers = subscribers[typeid(TEvent)].get();
        if (handlers) {
            for (auto it = handlers->begin(); it != handlers->end(); it++) {
                auto handler = it->get();
                TEvent event(std::forward<TArgs>(args)...);
                handler->Execute(event);
            }
        }
    }

    // Clear the subscribers list
    void Reset() {
        subscribers.clear();
    }
};

#endif