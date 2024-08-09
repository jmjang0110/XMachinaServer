#include "pch.h"
#include "GameSkill.h"
#include "GameManager.h"


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
	// Update�� �Ҹ� �������� CoolTime �� ���� ���̹Ƿ� 
	// State�� �ٽ� Possible �� �����Ѵ�. 
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

bool GameSkill::OnSkill(float playerTotalPhero)
{
	// ��� �Ұ� 
	if (playerTotalPhero < mConsumePheroAmount)
		return false;

	// ��밡�� 
	if (S_GetState() == GameSkill::State::Possible) {

		switch (mSkillType)
		{
		case FBProtocol::PLAYER_SKILL_TYPE_IMPOSSIBLE:
			break;
		case FBProtocol::PLAYER_SKILL_TYPE_CLOACKING:
			break;
		case FBProtocol::PLAYER_SKILL_TYPE_IR_DETECTOR:
			break;
		case FBProtocol::PLAYER_SKILL_TYPE_MIND_CONTROL:
			InitSkill_MindControl();
			break;
		case FBProtocol::PLAYER_SKILL_TYPE_SHIELD:
			break;
		default:
			assert(0);
			break;
		}
		S_SetState(GameSkill::State::Active);
		auto duration_in_seconds   = std::chrono::duration<float>(mCoolTime);
		auto coolTimeDuration = std::chrono::duration_cast<std::chrono::system_clock::duration>(duration_in_seconds);
		GameObject::RegisterUpdate(coolTimeDuration); // coolTime �Ŀ� ȣ���
	}
	else {
		// State : Active, Impossible �� ���� ��� �Ұ�  
		return false;
	}

	return true;
}

void GameSkill::InitSkill_MindControl()
{
	if (mMindControlMonster == nullptr)
		assert(0);

	//mMindControlMonster->

}

void GameSkill::InitSkill_Shield()
{
}
