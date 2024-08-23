#pragma once
/// +-------------------------------
///		     View List 
/// ________________________________
/// 
///		Player �� ViewList ����
/// Playe�� View Range �þ߿� ����
/// GameObject �� �����Ѵ�.
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