#pragma once

#include "Script_Stat.h"

namespace PlayerInfo {
	enum class Movement : DWORD {
		None   = 0x00,
		Stand  = 0x01,
		Sit    = 0x02,
		Walk   = 0x10,
		Run    = 0x20,
		Sprint = 0x40
	};

	enum class Rotate : DWORD {
		None = 0x00,
		Stop = 0x01,
		Cw   = 0x02,
		CCw  = 0x10,
	};

	namespace Speed {
		const float SitWalkSpeed   = 1.5f;
		const float StandWalkSpeed = 2.2f;
		const float RunSpeed       = 5.f;
		const float SprintSpeed    = 8.f;
	}
}

class Script_PlayerStat : public Script_Stat
{
private:

	/// +-------------------------------------------
	///	 >> ���������� Player Info 
	/// -------------------------------------------+
	SPtr_GameObject	mTarget           = {};		// self GameObject
	Vec3			mRespawn_Position = {};		// ������ ����
	int				mScore            = {};

	/// +-------------------------------------------
	///	 >> Phero
	/// -------------------------------------------+
	float mStartPheroAmount{};
	float mCurrPheroAmount{};
	float mMaxPheroAmount{};
	float mPheroRegenRate{};

	/// +-------------------------------------------
	///	 >> Weapon
	/// -------------------------------------------+
	int mCrntWeaponNum{};
	int mNextWeaponNum{};

public:

	Script_PlayerStat();
	Script_PlayerStat(UINT32 id);
	~Script_PlayerStat();


public:

	/// +------------------------------
	///			virtual function 
	/// ------------------------------+
	virtual void Activate();
	virtual void DeActivate();

	virtual bool WakeUp();
	virtual bool Start();
	virtual bool Update();
	virtual void OnDestory();

	/// +------------------------------
	///		 Stat : virtual function 
	/// ------------------------------+
	virtual void Attack();
	virtual void Dead();
	virtual bool Hit(float damage, SPtr_GameObject instigator = nullptr);

public:
	// player�� [pos]�� ��ġ��Ű�� �ش� ��ġ�� ������ �������� �����Ѵ�.
	void SetSpawn(const Vec3& pos);



public:
	/// +-------------------------------------------
	///	 >> Phero
	/// -------------------------------------------+
	virtual void AddPheroAmount(float pheroAmount);
	virtual bool ReducePheroAmount(float pheroCost, bool checkOnly = false);

	/// +-------------------------------------------
	///	 >> Weapon
	/// -------------------------------------------+
	
};

