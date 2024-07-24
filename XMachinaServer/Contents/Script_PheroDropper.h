#pragma once
#include "Script.h"


namespace PheroDropInfo {
	extern std::vector<Vec3> Offsets;  // ���� ���� ����
}
class GameObject;
class Script_PheroDropper : public Script
{
private:
    std::vector<SPtr<GameObject>> mPheros{};

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

private:

    void Shuffle_OdffsetDistIndexList();
	// mPheros ���͸� ��ȯ�ϴ� getter �Լ�
	const std::vector<SPtr<GameObject>>& getPheros() const {
		return mPheros;
	}


};

