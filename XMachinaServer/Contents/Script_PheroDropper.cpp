#include "pch.h"
#include "Script_PheroDropper.h"
#include "Script_Phero.h"
#include "Script_Enemy.h"
#include "Script_EnemyStat.h"


#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "Collider.h"
#include "Animation.h"
#include "ResourceManager.h"


#include "DBController.h"
#include "DB_Phero.h"
#include "DB_PheroDropInfo.h"

Script_PheroDropper::Script_PheroDropper()
{
	mKey = uint32_t_ScriptKey(ScriptKey::PheroDropper);
}

Script_PheroDropper::Script_PheroDropper(SPtr<GameObject> owner)
	: Script(owner, uint32_t_ScriptKey(ScriptKey::PheroDropper))
{
}

Script_PheroDropper::~Script_PheroDropper()
{
	mPheros.clear();
}

SPtr<Component> Script_PheroDropper::Clone(SPtr<Component> target)
{
	// 기본 클래스의 Clone 호출
	Script::Clone(target);

	// Script_PheroDropper 타입으로 캐스팅
	auto script_phero_dropper = std::dynamic_pointer_cast<Script_PheroDropper>(target);
	if (script_phero_dropper) {
		// Script_PheroDropper의 멤버들을 복사
		this->mPherosString = script_phero_dropper->mPherosString;
		for (int i = 0; i < script_phero_dropper->mPheros.size(); ++i) {
			SPtr<GameObject> phero = script_phero_dropper->mPheros[i]->Clone();
			this->mPheros.push_back(phero);
		}
	}
	else {
		std::cout << "Clone failed: other is not of type Script_PheroDropper" << std::endl;
	}

	return target;
}

void Script_PheroDropper::Clone(SPtr<GameObject> target)
{
	Script::Clone(target);

	// Add a new Script_PheroDropper instance to the GameObject
	auto clonedScript = target->AddScript<Script_PheroDropper>();
	// Clone the current script into the new script
	this->Clone(clonedScript);
}

void Script_PheroDropper::Start()
{
	Init();
}

SPtr<GameObject> Script_PheroDropper::CreatePheroObject(int pheroUnique_id, int phero_level)
{
	/// > Phero Object 
	std::shared_ptr<GameObject> phero = std::make_shared<GameObject>(pheroUnique_id);
	phero->AddComponent<Transform>(Component::Type::Transform);
	phero->AddComponent<Collider>(Component::Type::Collider);
	auto phero_entity = phero->SetScriptEntity<Script_Phero>();
	Vec3 offsetDist = Math::RandVec2(pheroUnique_id).xz();
	phero_entity->SetOffsetDist(offsetDist);

	/// > Phero Level 
	// PheroDropInfo::Min_Phero_Level + std::rand() % PheroDropInfo::Max_Phero_Level; // 1 2 3 
	phero_entity->SetLevel(phero_level);

	return phero;
}

void Script_PheroDropper::Init()
{
	mPheroLevel                = mOwner->GetScriptEntity<Script_Enemy>()->GetStat_PheroLevel();
	std::string AllPherosState = {};
	int			monsterID      = GetOwner()->GetID();

	// 각 레벨별 페로 수 계산 (50%: Level 1, 30%: Level 2, 20%: Level 3)
	int level1Count = 0;
	int level2Count = 0;
	int level3Count = 0;

#ifdef SET_DATA_FROM_DATABASE

	SPtr<DB_PheroDropInfo> pheroDropInfoDB = RESOURCE_MGR->GetPheroDropInfo(mPheroLevel);
	int pherosCount = pheroDropInfoDB->MinPheroDropCount + std::rand() % (pheroDropInfoDB->MaxPheroDropCount - pheroDropInfoDB->MinPheroDropCount);

	for (int i = 0; i < pherosCount; ++i) {
		int randValue = std::rand() % 100; // 0에서 99 사이의 난수 생성

		if (randValue < pheroDropInfoDB->L1PheroDropRate) {
			++level1Count; // 레벨 1
		}
		else if (randValue < pheroDropInfoDB->L1PheroDropRate + pheroDropInfoDB->L2PheroDropRate) {
			++level2Count; // 레벨 2
		}
		else {
			++level3Count; // 레벨 3
		}
	}

#else

	int rand = PheroDropInfo::Min_Phero_Drop_Num + std::rand() % (PheroDropInfo::Max_Phero_Drop_Num - PheroDropInfo::Min_Phero_Drop_Num);
	mPheros.reserve(rand);
	int pherosCount = PheroDropInfo::Min_Phero_Drop_Num + std::rand() % (PheroDropInfo::Max_Phero_Drop_Num - PheroDropInfo::Min_Phero_Drop_Num);

	for (int i = 0; i < pherosCount; ++i) {
		int randValue = std::rand() % 100; // 0에서 99 사이의 난수 생성

		if (randValue < 40) {
			++level1Count; // 레벨 1
		}
		else if (randValue < 60) {
			++level2Count; // 레벨 2
		}
		else {
			++level3Count; // 레벨 3
		}
	}

#endif

	// 정렬된 순서로 페로 생성 (레벨 1 -> 레벨 2 -> 레벨 3)
	for (int i = 0; i < level1Count; ++i) {
		mPheros.push_back(CreatePheroObject(CreateUniquePheroID(monsterID, i), 1));
		AllPherosState += "1"; // 레벨 1 상태 추가
	}

	for (int i = 0; i < level2Count; ++i) {
		mPheros.push_back(CreatePheroObject(CreateUniquePheroID(monsterID, i + level1Count), 2));
		AllPherosState += "2"; // 레벨 2 상태 추가
	}

	for (int i = 0; i < level3Count; ++i) {
		mPheros.push_back(CreatePheroObject(CreateUniquePheroID(monsterID, i + level1Count + level2Count), 3));
		AllPherosState += "3"; // 레벨 3 상태 추가
	}

	// 모든 페로 상태 설정
	mPherosString = AllPherosState;
	//LOG_MGR->Cout("PHERO : ", mPherosString, "\n");
}

int Script_PheroDropper::CalculatePercentage(int totalNumber, double percentage)
{
	return static_cast<int>(std::round(totalNumber * (percentage / 100.0)));
}

void Script_PheroDropper::Shuffle_OdffsetDistIndexList()
{

}

int Script_PheroDropper::CreateUniquePheroID(int monster_id, int phero_index)
{
	int pheroUniqueID = (monster_id - 1) * (PheroDropInfo::Max_Phero_Drop_Num + 1) + phero_index;
	return pheroUniqueID;
}

void Script_PheroDropper::SetPherosPos(Vec3& monsterDeadPoint)
{
	for (int i = 0; i < mPheros.size(); ++i) {
		Vec3 destPos = monsterDeadPoint + mPheros[i]->GetScriptEntity<Script_Phero>()->GetOffsetDist();
		mPheros[i]->GetTransform()->SetPosition(destPos);

	}
}
