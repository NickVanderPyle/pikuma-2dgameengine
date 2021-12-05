#pragma once

#include "Event.h"

class IEventCallback {
private:
    virtual void Call(Event &e) = 0;

public:
    virtual ~IEventCallback() = default;

    void Execute(Event &e) {
        Call(e);
    }
};