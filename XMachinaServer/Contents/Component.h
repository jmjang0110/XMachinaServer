#pragma once

#include "GameEntity.h"


namespace ComponentType {
	constexpr UINT16 None      = 0;

	constexpr UINT16 Tarnsform = 1;
	constexpr UINT16 Collider  = 2;

	constexpr UINT16 End       = 3;
}



class Component : public GameEntity
{
private:

public:
	Component();
	Component(UINT32 id);
	~Component();


public:
	virtual bool WakeUp();
	virtual bool Start();
	virtual bool Update();

};

