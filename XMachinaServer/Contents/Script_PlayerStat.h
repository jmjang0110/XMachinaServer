#pragma once

#include "Script_Stat.h"

namespace PlayerInfo
{
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

	constexpr int BulletsMax = 30;
}

class Script_PlayerStat : public Script_Stat
{
private:
	SPtr<GamePlayer> mPlayerOwner = nullptr;

private:

	/// +-------------------------------------------
	///	 >> ���������� Player Info 
	/// -------------------------------------------+
	SPtr<GameObject>	mTarget           = {};		// self GameObject
	Vec3				mRespawn_Position = {};		// ������ ����
	int					mScore            = {};

	/// +-------------------------------------------
	///	 >> Phero
	/// -------------------------------------------+
	float mStartPheroAmount{};
	float mMaxPheroAmount{};

	float mCurrPheroAmount{};	Lock::SRWLock Lock_CurrPheroAmount;
	float mPheroRegenRate{};

	/// +-------------------------------------------
	///	 >> Weapon
	/// -------------------------------------------+
	int mCrntWeaponNum{};
	int mNextWeaponNum{};

	/// +-------------------------------------------
	///	 >> Skill
	/// -------------------------------------------+


public:

	Script_PlayerStat();
	Script_PlayerStat(SPtr<GameObject> owner, ScriptInfo::Type type);
	~Script_PlayerStat();


public:

	/// +------------------------------
	///			virtual function 
	/// ------------------------------+
	virtual void Clone(SPtr<Component> other) ;


	virtual bool Start();

	/// +------------------------------
	///		 Stat : virtual function 
	/// ------------------------------+

public:
	// player�� [pos]�� ��ġ��Ű�� �ش� ��ġ�� ������ �������� �����Ѵ�.
	void SetSpawn(const Vec3& pos);



public:
	/// +-------------------------------------------
	///	 >> Phero
	/// -------------------------------------------+
	virtual void AddPheroAmount(float pheroAmount);
	virtual bool ReducePheroAmount(float pheroCost);

	void S_SetCurrPheroAmount(float phero);
	float S_GetCurrPheroAmount();


	/// +-------------------------------------------
	///	 >> Weapon
	/// -------------------------------------------+
	
};

