#pragma once

#include "Script_PlayerStat.h"
#include "Skill.h"
#include "GameBullet.h"



class GamePlayer;
class Script_Player : public Script_PlayerStat
{
private:
	Skill* mSkills[static_cast<UINT8>(SkillInfo::Type::End)] = {};
	SPtr<GameBullet> mBullets[PlayerInfo::BulletsMax]        = {};


public:  
	Script_Player();
	Script_Player(SPtr<GamePlayer> owner, ScriptInfo::Type type);
	Script_Player(SPtr<GameObject> owner, ScriptInfo::Type type);
	~Script_Player();


public:
	virtual void Clone(SPtr<Component> other) ;

	virtual void Activate();
	virtual void DeActivate();

	virtual bool WakeUp()	override;
	virtual bool Start()	override;
	virtual bool Update()	override;
	virtual void OnDestroy() override;


	virtual bool Attack()	override;
	virtual void Dead()		override;
	virtual bool Hit(float damage, SPtr_GameObject instigator = nullptr) override;

public:
	Skill* GetSkill(SkillInfo::Type skill_type) { return mSkills[static_cast<UINT8>(skill_type)]; }

};

