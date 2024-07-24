#pragma once

#include "Gameinfo.h"

struct ObjectSnapShot
{
	SPtr<GameObject>				Owner{};
	int								ID{};
	GameObjectInfo::Type			Type = GameObjectInfo::Type::None;

	ObjectSnapShot() {};
	~ObjectSnapShot() {};
};