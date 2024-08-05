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
	SetSNS_State(GameSkill::State::Possible);
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
	if (GetSNS_State() == GameSkill::State::Possible) {

		SetSNS_State(GameSkill::State::Active);
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
