#include "pch.h"
#include "Script_PheroDropper.h"
#include "Script_Phero.h"

#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "Collider.h"
#include "Animation.h"

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

void Script_PheroDropper::Init()
{
	std::string AllPherosState = {};
	/// > 5 ~ 30 (min ~ max) - ( pheros Count )
	int rand = PheroDropInfo::Min_Phero_Drop_Num + std::rand() % (PheroDropInfo::Max_Phero_Drop_Num - PheroDropInfo::Min_Phero_Drop_Num);
	mPheros.reserve(rand);

	int Level1_PDR = std::rand() % rand;
	int Level2_PDR = std::rand() % (rand - Level1_PDR);
	int Level3_PDR = rand - Level1_PDR - Level2_PDR;

	for (int i = 0; i < Level1_PDR; ++i) {
		/// +---------------------------------------------------------------
		///							CREATE PHERO 
		/// ---------------------------------------------------------------+
		
		/// > Phero Unique ID 
		int monsterID     = GetOwner()->GetID();
		int phero_index   = i;
		int pheroUniqueID = CreateUniquePheroID(monsterID, phero_index);

		/// > Phero Object 
		std::shared_ptr<GameObject> phero = std::make_shared<GameObject>(pheroUniqueID);
		phero->AddComponent<Transform>(Component::Type::Transform);
		phero->AddComponent<Collider>(Component::Type::Collider);
		auto phero_entity = phero->SetScriptEntity<Script_Phero>();
		Vec3 offsetDist = Math::RandVec2(pheroUniqueID).xz();
		phero_entity->SetOffsetDist(offsetDist);

		/// > Phero Level 
		int level = 1;// PheroDropInfo::Min_Phero_Level + std::rand() % PheroDropInfo::Max_Phero_Level; // 1 2 3 
		phero_entity->SetLevel(level);
		mPheros.push_back(phero);

		/// > Store Phero states (All) "11231323123" = [1 lv]-(0) [1 lv]-(1) [2 lv]-(2)  ... [3 lv]
		AllPherosState += std::to_string(level);
	}


	for (int i = Level1_PDR; i < Level1_PDR + Level2_PDR; ++i) {
		/// +---------------------------------------------------------------
		///							CREATE PHERO 
		/// ---------------------------------------------------------------+

		/// > Phero Unique ID 
		int monsterID = GetOwner()->GetID();
		int phero_index = i;
		int pheroUniqueID = CreateUniquePheroID(monsterID, phero_index);

		/// > Phero Object 
		std::shared_ptr<GameObject> phero = std::make_shared<GameObject>(pheroUniqueID);
		phero->AddComponent<Transform>(Component::Type::Transform);
		phero->AddComponent<Collider>(Component::Type::Collider);
		auto phero_entity = phero->SetScriptEntity<Script_Phero>();

		Vec3 offsetDist = Math::RandVec2(pheroUniqueID).xz();
		phero_entity->SetOffsetDist(offsetDist);

		/// > Phero Level 
		int level = 2;// PheroDropInfo::Min_Phero_Level + std::rand() % PheroDropInfo::Max_Phero_Level; // 1 2 3 
		phero_entity->SetLevel(level);
		mPheros.push_back(phero);

		/// > Store Phero states (All) "11231323123" = [1 lv]-(0) [1 lv]-(1) [2 lv]-(2)  ... [3 lv]
		AllPherosState += std::to_string(level);
	}


	for (int i = Level1_PDR + Level2_PDR; i < rand; ++i) {
		/// +---------------------------------------------------------------
		///							CREATE PHERO 
		/// ---------------------------------------------------------------+

		/// > Phero Unique ID 
		int monsterID = GetOwner()->GetID();
		int phero_index = i;
		int pheroUniqueID = CreateUniquePheroID(monsterID, phero_index);

		/// > Phero Object 
		std::shared_ptr<GameObject> phero = std::make_shared<GameObject>(pheroUniqueID);
		phero->AddComponent<Transform>(Component::Type::Transform);
		phero->AddComponent<Collider>(Component::Type::Collider);
		auto phero_entity = phero->SetScriptEntity<Script_Phero>();
		Vec3 offsetDist = Math::RandVec2(pheroUniqueID).xz();
		phero_entity->SetOffsetDist(offsetDist);

		/// > Phero Level 
		int level = 3;// PheroDropInfo::Min_Phero_Level + std::rand() % PheroDropInfo::Max_Phero_Level; // 1 2 3 
		phero_entity->SetLevel(level);
		mPheros.push_back(phero);

		/// > Store Phero states (All) "11231323123" = [1 lv]-(0) [1 lv]-(1) [2 lv]-(2)  ... [3 lv]
		AllPherosState += std::to_string(level);
	}

	/// > Set Pheros States ( All Pheros Info )
	mPherosString = AllPherosState;
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
