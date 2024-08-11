#include "pch.h"
#include "GameSkill.h"
#include "GameManager.h"
#include "Script_EnemyController.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "Script_Skill.h"
#include "Script_SkillCloaking.h"
#include "Script_SkillMindControl.h"
#include "Script_SkillShield.h"

#include "GameMonster.h"





GameSkill::GameSkill() 
	: GameObject()

{
}

GameSkill::GameSkill(UINT32 sessionID) 
	: GameObject(sessionID)

{
}

GameSkill::GameSkill(UINT32 sessionID, SPtr<GamePlayer> owner) 
	: GameObject(sessionID)
{
	mOwnerPlayer = owner;

}

GameSkill::~GameSkill()
{
}

void GameSkill::Update()
{
	GameObject::Update();

	// Update가 불릴 시점에는 CoolTime 이 지난 후이므로 
	// State를 다시 Possible 로 변경한다. 
	

}

void GameSkill::Activate()
{
	GameObject::Activate();

	mActivate_Ref.fetch_add(1);

	if (mActivate_Ref.load() == 1) {
		GameObject::RegisterUpdate();
	}
}

void GameSkill::DeActivate()
{
	GameObject::DeActivate();

	mActivate_Ref.fetch_sub(1);
}

void GameSkill::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{
	MEMORY->Delete(overlapped);

	Update();
}

bool GameSkill::OnSkill(float playerTotalPhero, SPtr<GameMonster> monster)
{
	// 사용 불가 
	if (playerTotalPhero < mConsumePheroAmount)
		return false;

	GameSkill::State skillState = GetState();
	switch (skillState)
	{
	case GameSkill::State::Possible: {
		SetState(GameSkill::State::Active);
		Activate();

		if (mSkillType == FBProtocol::PLAYER_SKILL_TYPE_MIND_CONTROL) {
			if (monster) {
				auto mindC = dynamic_cast<Script_SkillMindControl*>(mSkillScript);
				mindC->Init(monster);
			}
		}
	}
		break;
	default:
		break;
	}


	return true;
}

void GameSkill::RegisterUpdate(float duration)
{
	auto duration_in_seconds = std::chrono::duration<float>(duration);
	auto time = std::chrono::duration_cast<std::chrono::system_clock::duration>(duration_in_seconds);
	GameObject::RegisterUpdate(time); // coolTime 후에 호출됨
}

void GameSkill::SetCoolTime(float time)
{
	mSkillScript->SetCoolTime(time);
}

void GameSkill::SetActiveDurationTime(float time)
{
	mSkillScript->SetDurationTime(time);
}