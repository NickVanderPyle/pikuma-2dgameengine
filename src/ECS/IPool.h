#pragma once

class IPool {
public:
    virtual ~IPool() = default;

    virtual void RemoveEntityFromPool(int entityId) = 0;
};
