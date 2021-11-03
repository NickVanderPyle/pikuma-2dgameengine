#ifndef ENTITY_H
#define ENTITY_H

class Entity
{
private:
	int id;
public:
	Entity(int id)
		: id(id)
	{};

	Entity(const Entity &entity) = default;

	int GetId() const;

	Entity &operator=(const Entity &other) = default;

	bool operator==(const Entity &other) const
	{ return id == other.id; }

	bool operator!=(const Entity &other) const
	{ return id != other.id; }

	bool operator>(const Entity &other) const
	{ return id > other.id; }

	bool operator<(const Entity &other) const
	{ return id < other.id; }
};

#endif //ENTITY_H
