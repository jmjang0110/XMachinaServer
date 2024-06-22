#include "pch.h"
#include "Entity.h"

Entity::Entity()
{
}

Entity::Entity(std::string name)
{
	mName = name;
	mID = -1;
}

Entity::~Entity()
{
}

std::string Entity::GetName()
{
	return mName;
}

void Entity::SetName(std::string name)
{
	mName = name;
}

UINT64 Entity::GetID()
{
	return mID;
}

void Entity::SetName(UINT64 id)
{
	mID = id;
}
