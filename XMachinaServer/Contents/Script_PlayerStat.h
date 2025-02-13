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
protected:
	Vec3				mRespawn_Position	= {};		// 리스폰 지점
	int					mScore				= {};

	/// +-------------------------------------------
	///	 >> Phero
	/// -------------------------------------------+	
	float				mStartPheroAmount	= {};
	float				mMaxPheroAmount		= {};

	float				mPheroRegenRate		= {};

	/// +-------------------------------------------
	///	 >> Speed
	/// -------------------------------------------+	
	float				mVelocity			= {};

public:
	Script_PlayerStat();
	Script_PlayerStat(SPtr<GameObject> owner);
	virtual ~Script_PlayerStat();


public:
	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);

	virtual void Start();

public:
	// player를 [pos]로 위치시키고 해당 위치를 리스폰 지점으로 설정한다.
	void SetSpawn(const Vec3& pos);
	void SetVelocity(float velocity) { mVelocity = velocity; }

	float GetVelocity() { return mVelocity; }
public:


	
};

