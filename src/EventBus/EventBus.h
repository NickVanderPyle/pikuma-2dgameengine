#pragma once

#include <map>
#include <typeindex>
#include <list>
#include "../Logger/Logger.h"
#include "Event.h"
#include "IEventCallback.h"
#include "EventCallback.h"

typedef std::list<std::unique_ptr<IEventCallback>> HandlerList;

class EventBus {
private:
    std::map<std::type_index, std::unique_ptr<HandlerList>> subscribers;

public:
    EventBus() {
        Logger::Log("EventBus constructor called.");
    }

    ~EventBus() {
        Logger::Log("EventBus destructor called.");
    }

    void Reset() {
        subscribers.clear();
    }

    /*
     * Subscribe to an event type <T>
     * In our implementation a listener subscribes to an event
     * Example: eventBus->SubscribeToEvent<CollisionEvent>(this, &onCollision);
     */
    template<typename TEvent, typename TOwner>
    void SubscribeToEvent(TOwner *ownerInstance, void (TOwner::*callbackFunction)(TEvent &)) {
        if (subscribers[typeid(TEvent)].get() == nullptr) {
            subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
        }
        auto subscriber = std::make_unique<EventCallback<TOwner, TEvent>>(ownerInstance, callbackFunction);
        subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
    }

    /*
     * Emit an event type <T>
     * In our implementation, as soon as something emits an event
     * we execute all the callback functions.
     * Example: eventBus->EmitEvent<CollisionEvent>(player, enemy);
     */
    template<typename TEvent, typename  ...TArgs>
    void EmitEvent(TArgs &&...args) {
        auto handlers = subscribers[typeid(TEvent)].get();
        if (handlers == nullptr) {
            return;
        }

        for (auto it = handlers->begin(); it != handlers->end(); it++) {
            auto handler = it->get();
            TEvent event(std::forward<TArgs>(args)...);
            handler->Execute(event);
        }
    }
};