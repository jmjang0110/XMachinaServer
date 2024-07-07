#pragma once

#include "Script.h"
#include "GameObject.h"


/// +-------------------------------
///		     Script_EnemyController 
/// ________________________________
///	> [ ���� ] 
///		- Enemy Controller Script�� ���� �ִ� ��ü�� 
///		- �ڽ��� Enemy �̸� 
/// ________________________________
/// 
/// 

namespace EnemyInfo
{
	struct Stat 
	{

	};

	enum class State : UINT8 {
		Idle = 0,
		Walk,
		GetHit,
		Attack,
		Death,
		END,
	};
}


class Script_EnemyController : public Script
{
private:
	EnemyInfo::Stat		mStat   = {};
	EnemyInfo::State	mState  = EnemyInfo::State::Idle;

	GameObject*			mTarget = {};
	std::stack<Vec3>	mPaths  = {};


public:
	virtual bool WakeUp()	override;
	virtual bool Start()	override;
	virtual bool Update()	override;

public:
	void Reset();
	void ForceSetTarget(GameObject* target) { mTarget = target; }
public:
	Script_EnemyController();
	Script_EnemyController(UINT32 id);
	~Script_EnemyController();

};

