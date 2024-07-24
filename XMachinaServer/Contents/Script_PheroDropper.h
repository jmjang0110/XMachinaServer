#pragma once
#include "Script.h"


namespace PheroDropInfo {
	extern std::vector<Vec3> Offsets;  // 전역 변수 선언
}
class GameObject;
class Script_PheroDropper : public Script
{
private:
    std::vector<GameObject*> mPheros{};
    std::vector<int> mOffsetdistIndexList{};

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

private:

    void Shuffle_OdffsetDistIndexList();



};

