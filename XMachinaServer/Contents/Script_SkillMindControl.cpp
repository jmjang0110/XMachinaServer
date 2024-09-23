#include "pch.h"
#include "Script_SkillMindControl.h"
#include "GameObject.h"
#include "Script_EnemyController.h"
#include "Script_Enemy.h"


Script_SkillMindControl::Script_SkillMindControl(SPtr<GameObject> owner)
	: Script_Skill(owner)
{
	mSkillType = FBProtocol::PLAYER_SKILL_TYPE_MIND_CONTROL;
#ifdef SET_DATA_FROM_DATABASE
	Script_Skill::SetDataFromDataBase("MindControl");
	mCoolTime -= 1.f;
	mActiveDuration -= 1.f;

#else

#endif
}

Script_SkillMindControl::~Script_SkillMindControl()
{
}

SPtr<Component> Script_SkillMindControl::Clone(SPtr<Component> target)
{
	// Try to cast the target to Script_Anglerox
	auto clonedScript = std::dynamic_pointer_cast<Script_SkillMindControl>(target);
	if (clonedScript)
	{
		// Call the base class Clone method first
		Script_Skill::Clone(clonedScript);
		return clonedScript;
	}
	else
	{
		std::cout << "Clone failed: target is not of type Script_SkillMindControl" << std::endl;
		return nullptr;
	}
}

void Script_SkillMindControl::Clone(SPtr<GameObject> target)
{
	// Add a new Script_Anglerox instance to the GameObject
	auto clonedScript = target->AddScript<Script_SkillMindControl>();
	// Clone the current script into the new script
	this->Clone(clonedScript);
}

void Script_SkillMindControl::Update()
{
	Script_Skill::Update();

	switch (mSkillState)
	{
	case SkillState::Impossible:
	case SkillState::Possible:
		break;
	case SkillState::Active:
	{
		mSkillState = SkillState::CoolTime_Start;
		mOwner->RegisterUpdate(mActiveDuration); // call after activeduration time
	}
	break;
	case SkillState::CoolTime_Start:
	{
		mSkillState = SkillState::CoolTime_End;
		mOwner->RegisterUpdate(mCoolTime);		// call after cool time
	}
	break;
	case SkillState::CoolTime_End:
	{
		mSkillState = SkillState::Possible;
		mOwner->DeActivate();
	}
	break;
	default:
		assert(0);
		break;
	}
}

void Script_SkillMindControl::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{
	MEMORY->Delete(overlapped);

	mOwner->Update();
}

void Script_SkillMindControl::Init(SPtr<GameObject> monster)
{
	if (monster == nullptr)
		assert(0);

	auto Invoker = GetOwnerPlayer();
	auto entity  = monster->GetScriptEntity<Script_Enemy>();
	if (entity) {
		entity->GetController()->OnMindControl();
		entity->GetController()->SetInvoker(Invoker);
	}
	mMindControlMonster = monster;

}
