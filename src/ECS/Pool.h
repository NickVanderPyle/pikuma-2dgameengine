#pragma once

#include <vector>
#include "IPool.h"

template<typename T>
class Pool : public IPool {
private:
    std::vector<T> data;
    int size;

    std::unordered_map<int, int> entityIdToIndex;
    std::unordered_map<int, int> indexToEntityId;

public:
    Pool(int capacity = 100) {
        size = 0;
        data.resize(capacity);
    }

    virtual ~Pool() {};

    bool IsEmpty() const { return size == 0; }

    int GetSize() const { return size; }

    void Resize(int size) { data.resize(size); }

    void Clear() {
        size = 0;
        data.clear();
        entityIdToIndex.clear();
        indexToEntityId.clear();
    }

    void Add(T object) {
        data.push_back(object);
    }

    void Set(int entityId, T object) {
        if (entityIdToIndex.find(entityId) != entityIdToIndex.end()) {
            int index = entityIdToIndex[entityId];
            data[index] = object;
        } else {
            int index = size;
            entityIdToIndex.emplace(entityId, index);
            indexToEntityId.emplace(index, entityId);
            if (index >= data.capacity()) {
                data.resize(size * 2);
            }
            data[index] = object;
            size++;
        }
    }

    void Remove(int entityId) {
        int indexOfRemoved = entityIdToIndex[entityId];
        int indexOfLast = size - 1;
        data[indexOfRemoved] = data[indexOfLast];

        int entityIdOfLastElement = indexToEntityId[indexOfLast];
        entityIdToIndex[entityIdOfLastElement] = indexOfRemoved;
        indexToEntityId[indexOfRemoved] = entityIdOfLastElement;

        entityIdToIndex.erase(entityId);
        indexToEntityId.erase(indexOfLast);

        size--;
    }

    void RemoveEntityFromPool(int entityId) override {
        if (entityIdToIndex.find(entityId) != entityIdToIndex.end()) {
            Remove(entityId);
        }
    }

    T &Get(int entityId) {
        int index = entityIdToIndex[entityId];
        return static_cast<T &>(data[index]);
    }

    T &operator[](unsigned int idx) { return data[idx]; }
};
