#include "pch.h"
#include "ViewList.h"
#include "GameMonster.h"	
#include "GamePlayer.h"


bool ViewList::TryInsertPlayer(UINT32 key, SPtr<GamePlayer> player)
{
	auto monsterIt = VL_Players.find(key);
	// 이미 존재한다.
	if (monsterIt != VL_Players.end())
		return false;
	else
		VL_Players.insert(std::make_pair(key, player));

	return true;

}

bool ViewList::TryInsertMonster(UINT32 key, SPtr<GameMonster> monster, bool DoActivate)
{
	auto monsterIt = VL_Monsters.find(key);
	// 이미 존재한다.
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

bool ViewList::RemoveMonster(UINT32 key)
{
	auto monsterIt = VL_Monsters.find(key);
	if (monsterIt != VL_Monsters.end())
	{
		monsterIt->second->DeActivate();
		VL_Monsters.erase(monsterIt);
		return true;
	}
	return false;
}

void ViewList::Clear()
{
	for (auto& it : VL_Monsters)
		it.second->DecreaseRef();

	VL_Monsters.clear();
	VL_Players.clear();
}