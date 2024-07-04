#include "pch.h"
#include "Script.h"

Script::Script()
	: GameEntity()
{
}

Script::Script(UINT32 id)
	: GameEntity(id)
{
}

Script::~Script()
{
}

bool Script::WakeUp()
{
	return false;
}

bool Script::Start()
{
	return false;
}

bool Script::Update()
{
	return false;
}
