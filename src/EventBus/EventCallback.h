#pragma once

#include <functional>
#include "IEventCallback.h"

template<typename TOwner, typename TEvent>
class EventCallback : public IEventCallback {
private:
    typedef void (TOwner::*CallbackFunction)(TEvent &);

    TOwner *ownerInstance;
    CallbackFunction callbackFunction;

    virtual void Call(Event &e) override {
        std::invoke(callbackFunction, ownerInstance, static_cast<TEvent &>(e));
    }

public:
    EventCallback(TOwner *ownerInstance, CallbackFunction callbackFunction) {
        this->ownerInstance = ownerInstance;
        this->callbackFunction = callbackFunction;
    }

    virtual ~EventCallback() = default;
};