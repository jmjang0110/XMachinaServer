#pragma once
#include "GameEntity.h"


namespace ScriptType {
	constexpr UINT16 None = 0;

	constexpr UINT16 EnemyStat       = 1;
	constexpr UINT16 PlayerSTat      = 2;
	constexpr UINT16 EnemyController = 3;

	constexpr UINT16 End = 1;
	
}


class Script : public GameEntity
{
private:


public:
	Script();
	Script(UINT32 id);
	~Script();

public:
	virtual bool WakeUp();
	virtual bool Start();
	virtual bool Update();
};

