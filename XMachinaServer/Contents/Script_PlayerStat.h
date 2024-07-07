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


public:

	Script_PlayerStat();
	Script_PlayerStat(UINT32 id);
	~Script_PlayerStat();


public:
	virtual bool WakeUp()	override;
	virtual bool Start()	override;
	virtual bool Update()	override;

	virtual void Attack();
	virtual void Death();
	virtual bool Hit(float damage, GameObject* instigator = nullptr) override;

public:

};

