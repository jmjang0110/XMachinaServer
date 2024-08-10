#include "pch.h"
#include "GameSkill.h"
#include "GameManager.h"
#include "Script_EnemyController.h"


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
	// Update가 불릴 시점에는 CoolTime 이 지난 후이므로 
	// State를 다시 Possible 로 변경한다. 
	S_SetState(GameSkill::State::Possible);

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
}

bool GameSkill::OnSkill(float playerTotalPhero, SPtr<GameMonster> monster)
{
	// 사용 불가 
	if (playerTotalPhero < mConsumePheroAmount)
		return false;
	
	// 사용가능 
	if (S_GetState() == GameSkill::State::Possible) {

		switch (mSkillType)
		{
		case FBProtocol::PLAYER_SKILL_TYPE_IMPOSSIBLE:
			break;
		case FBProtocol::PLAYER_SKILL_TYPE_CLOACKING:
			InitSkill_Cloaking();
			break;
		case FBProtocol::PLAYER_SKILL_TYPE_IR_DETECTOR:
			break;
		case FBProtocol::PLAYER_SKILL_TYPE_MIND_CONTROL:
			InitSkill_MindControl(monster);
			break;
		case FBProtocol::PLAYER_SKILL_TYPE_SHIELD:
			InitSkill_Shield();
			break;
		default:
			assert(0);
			break;
		}
		S_SetState(GameSkill::State::Active);
		auto duration_in_seconds   = std::chrono::duration<float>(mCoolTime);
		auto coolTimeDuration = std::chrono::duration_cast<std::chrono::system_clock::duration>(duration_in_seconds);
		GameObject::RegisterUpdate(coolTimeDuration); // coolTime 후에 호출됨
	}
	else {
		// State : Active, Impossible 일 떄는 사용 불가  
		return false;
	}

	return true;
}

void GameSkill::InitSkill_Shield()
{
}

void GameSkill::InitSkill_Cloaking()
{
}

void GameSkill::InitSkill_MindControl(SPtr<GameMonster> monster)
{
	if (monster == nullptr)
		assert(0);

	monster->SetMindControlled(true);
	monster->GetEnemyController()->SetInvoker(std::dynamic_pointer_cast<GamePlayer>(GetOwnerPlayer()));
	SetMindControlMonster(monster);
}
