#pragma once

class GamePlayer;
class GameMonster;


/// +-------------------------------
///		     View List 
/// ________________________________
/// 
///		Player 의 ViewList 관리
/// Playe의 View Range 시야에 들어온
/// GamePlayer 와 GameMonster 를 관리한다.
/// -------------------------------+
class ViewList
{
public:
	std::unordered_map<UINT32, SPtr<GamePlayer>>	 VL_Players;
	std::unordered_map<UINT32, SPtr<GameMonster>>	 VL_Monsters;


public:
	bool TryInsertPlayer(UINT32 key, SPtr<GamePlayer> player);
	bool TryInsertMonster(UINT32 key, SPtr<GameMonster> monster);

	bool RemovePlayer(UINT32 key);
	bool RemoveMonster(UINT32 key);

	void Clear();
};