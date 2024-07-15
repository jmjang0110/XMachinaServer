#pragma once

#include "Script_PlayerStat.h"
#include "Skill.h"



class Script_Player : public Script_PlayerStat
{
private:
	Skill* mSkills[static_cast<UINT8>(SkillInfo::Type::End)]{};


public:
	Script_Player();
	Script_Player(SPtr<GameObject> owner, ScriptInfo::Type type);
	~Script_Player();


public:
	virtual void Activate();
	virtual void DeActivate();

	virtual bool WakeUp()	override;
	virtual bool Start()	override;
	virtual bool Update()	override;
	virtual void OnDestroy() override;


	virtual void Attack()	override;
	virtual void Dead()		override;
	virtual bool Hit(float damage, SPtr_GameObject instigator = nullptr) override;

};

