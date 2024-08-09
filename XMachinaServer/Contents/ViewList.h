#pragma once

class GamePlayer;
class GameMonster;


/// +-------------------------------
///		     View List 
/// ________________________________
/// 
///		Player �� ViewList ����
/// Playe�� View Range �þ߿� ����
/// GamePlayer �� GameMonster �� �����Ѵ�.
/// -------------------------------+
class ViewList
{
public:
	std::unordered_map<UINT32, SPtr<GamePlayer>>	 VL_Players;
	std::unordered_map<UINT32, SPtr<GameMonster>>	 VL_Monsters;


public:
	bool TryInsertPlayer(UINT32 key, SPtr<GamePlayer> player);
	bool TryInsertMonster(UINT32 key, SPtr<GameMonster> monster, bool DoActivate = true);

	bool RemovePlayer(UINT32 key);
	bool RemoveMonster(UINT32 key);

	void Clear();
};