#pragma once
#include "Script.h"

class Component;
class GameObject;
namespace PheroDropInfo {
	constexpr int Max_Phero_Drop_Num = 30;
	constexpr int Min_Phero_Drop_Num = 5;
	constexpr int Min_Phero_Level    = 1;
	constexpr int Max_Phero_Level    = 3;
};

class GameObject;
class Script_PheroDropper : public Script
{
private:
	std::string						mPherosString = {};
	std::vector<SPtr<GameObject>>	mPheros       = {};

public:
	Script_PheroDropper();
	Script_PheroDropper(SPtr<GameObject> owner);
	virtual ~Script_PheroDropper();

public:

	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);

	virtual void Start();


public:
	void Init();
	int	CalculatePercentage(int totalNumber, double percentage);

	/// ----------------- Get -----------------
	const std::vector<SPtr<GameObject>>& GetPheros()  {return mPheros;}
	std::string GetPherosString() { return mPherosString; }
	/// ----------------- Set -----------------
	void SetPherosPos(Vec3& monsterDeadPoint);

private:
    void	Shuffle_OdffsetDistIndexList();
	int		CreateUniquePheroID(int monster_id, int phero_index);
};

