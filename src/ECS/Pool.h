#ifndef POOL_H
#define POOL_H

#include <vector>
#include "IPool.h"

template<typename T>
class Pool: public IPool
{
private:
	std::vector<T> data;

public:
	Pool(int size = 100)
	{
		data.reserve(size);
	}

	virtual ~Pool() {};

	bool IsEmpty() const
	{ return data.empty(); }

	int GetSize() const
	{ return data.size(); }

	void Resize(int size)
	{ data.resize(size); }

	void Clear()
	{ data.clear(); }

	void Add(T object)
	{ data.push_back(object); }

	void Set(int idx, T object)
	{ data[idx] = object; }

	T &Get(int idx) const
	{ return data[idx]; }

	T &operator[](unsigned int idx)
	{ return data[idx]; }
};

#endif //POOL_H
