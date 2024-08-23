#pragma once
/// +-------------------------------
///		     View List 
/// ________________________________
/// 
///		Player 의 ViewList 관리
/// Playe의 View Range 시야에 들어온
/// GameObject 를 관리한다.
/// -------------------------------+

class GameObject;
class ViewList
{
public:
	float											 ViewRangeRadius = {};
	std::unordered_map<UINT32, SPtr<GameObject>>	 VL_Players      = {};
	std::unordered_map<UINT32, SPtr<GameObject>>	 VL_Monsters     = {};


public:
	bool TryInsertPlayer(UINT32 key, SPtr<GameObject> player);
	bool TryInsertMonster(UINT32 key, SPtr<GameObject> monster, bool DoActivate = true);

	bool RemovePlayer(UINT32 key);
	bool RemoveMonster(UINT32 key, bool DoActivate = true);

	void Clear();
};