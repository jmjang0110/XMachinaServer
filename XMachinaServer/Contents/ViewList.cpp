#include "pch.h"
#include "ViewList.h"
#include "GameObject.h"

bool ViewList::TryInsertPlayer(UINT32 key, SPtr<GameObject> player)
{
	auto monsterIt = VL_Players.find(key);
	if (monsterIt != VL_Players.end())
		return false;
	else
		VL_Players.insert(std::make_pair(key, player));

	return true;
}

bool ViewList::TryInsertMonster(UINT32 key, SPtr<GameObject> monster, bool DoActivate)
{
	auto monsterIt = VL_Monsters.find(key);
	if (monsterIt != VL_Monsters.end())
		return false;
	else
	{
		VL_Monsters.insert(std::make_pair(key, monster));
		if(DoActivate)
			monster->Activate();
	}
	return true;
}

bool ViewList::RemovePlayer(UINT32 key)
{
	auto playerIt = VL_Players.find(key);
	if (playerIt != VL_Players.end())
	{
		VL_Players.erase(playerIt);
		return true;
	}
	return false;
}

bool ViewList::RemoveMonster(UINT32 key, bool DoActivate)
{
	auto monsterIt = VL_Monsters.find(key);
	if (monsterIt != VL_Monsters.end())
	{
		if(DoActivate)
			monsterIt->second->DeActivate();
		VL_Monsters.erase(monsterIt);
		return true;
	}
	return false;
}

void ViewList::Clear()
{
	for (auto& it : VL_Monsters)
		it.second->DecreaseActivateRef();
	VL_Monsters.clear();
	VL_Players.clear();
}