#pragma once
#include "Script.h"

class GamePhero;

namespace PheroDropInfo {
	constexpr int Max_Phero_Drop_Num = 30;
	constexpr int Min_Phero_Drop_Num = 5;
	constexpr int Min_Phero_Level    = 1;
	constexpr int Max_Phero_Level    = 3;
};

class GameObject;
class Script_PheroDropper : public Script
{
public:

private:
    std::vector<SPtr<GamePhero>> mPheros{};

public:
	Script_PheroDropper();
	Script_PheroDropper(SPtr<GameObject> owner, ScriptInfo::Type type);
	~Script_PheroDropper();

public:
	/// +------------------------------
	///		  virtual function 
	/// ------------------------------+
	virtual void Clone(SPtr<Component> other) ;

	virtual void Activate(); 
	virtual void DeActivate();

	virtual bool WakeUp();
	virtual bool Start();
	virtual bool Update();
	virtual void OnDestroy();


	void Init();

public:
	void OnCollisionWithPlayer(int PlayerID);

	int CalculatePercentage(int totalNumber, double percentage);
	const std::vector<SPtr<GamePhero>>& GetPheros()  {return mPheros;}
	void SetPherosPos(Vec3& monsterDeadPoint);

private:

    void Shuffle_OdffsetDistIndexList();
	int CreateUniquePheroID(int monster_id, int phero_index);



};

