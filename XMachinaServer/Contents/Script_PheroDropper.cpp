#include "pch.h"
#include "Script_PheroDropper.h"
#include "GameObject.h"
#include "GamePhero.h"

#include "Component.h"
#include "Transform.h"
#include "Collider.h"
#include "Gameinfo.h"
#include "GameMonster.h"

#include "Script_Phero.h"

Script_PheroDropper::Script_PheroDropper()
{
}

Script_PheroDropper::Script_PheroDropper(SPtr<GameObject> owner, ScriptInfo::Type type)
	: Script(owner, type, static_cast<UINT32>(type))
{

}

Script_PheroDropper::~Script_PheroDropper()
{

	mPheros.clear();

}

void Script_PheroDropper::Clone(SPtr<Component> other) 
{
	Script::Clone(other);
	SPtr<Script_PheroDropper> otherScript = std::static_pointer_cast<Script_PheroDropper>(other);

	for (int i = 0; i < otherScript->GetPheros().size(); ++i) {
		SPtr<GamePhero> phero = otherScript->GetPheros()[i]->Clone();
		phero->SetID(otherScript->GetPheros()[i]->GetID());
		mPheros.push_back(phero);

	}
}

void Script_PheroDropper::Activate()
{
	Script::Activate();

}

void Script_PheroDropper::DeActivate()
{
	Script::DeActivate();

}

bool Script_PheroDropper::WakeUp()
{
	Script::WakeUp();


	return true;
}

bool Script_PheroDropper::Start()
{
	Script::Start();


	return true;
}

bool Script_PheroDropper::Update()
{
	Script::Update();

	return true;
}

void Script_PheroDropper::OnDestroy()
{
	Script::OnDestroy();

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
		std::shared_ptr<GamePhero> phero = std::make_shared<GamePhero>(pheroUniqueID);
		phero->AddComponent<Transform>(ComponentInfo::Type::Transform);
		phero->AddComponent<Collider>(ComponentInfo::Type::Collider);
		auto script = phero->AddScript<Script_Phero>(ScriptInfo::Type::Phero);

		Vec3 offsetDist = Math::RandVec2(pheroUniqueID).xz();
		phero->SetOffsetDist(offsetDist);

		/// > Phero Level 
		int level = 1;// PheroDropInfo::Min_Phero_Level + std::rand() % PheroDropInfo::Max_Phero_Level; // 1 2 3 
		script->SetLevel(level);
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
		std::shared_ptr<GamePhero> phero = std::make_shared<GamePhero>(pheroUniqueID);
		phero->AddComponent<Transform>(ComponentInfo::Type::Transform);
		phero->AddComponent<Collider>(ComponentInfo::Type::Collider);
		auto script = phero->AddScript<Script_Phero>(ScriptInfo::Type::Phero);

		Vec3 offsetDist = Math::RandVec2(pheroUniqueID).xz();
		phero->SetOffsetDist(offsetDist);

		/// > Phero Level 
		int level = 2;// PheroDropInfo::Min_Phero_Level + std::rand() % PheroDropInfo::Max_Phero_Level; // 1 2 3 
		script->SetLevel(level);
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
		std::shared_ptr<GamePhero> phero = std::make_shared<GamePhero>(pheroUniqueID);
		phero->AddComponent<Transform>(ComponentInfo::Type::Transform);
		phero->AddComponent<Collider>(ComponentInfo::Type::Collider);
		auto script = phero->AddScript<Script_Phero>(ScriptInfo::Type::Phero);

		Vec3 offsetDist = Math::RandVec2(pheroUniqueID).xz();
		phero->SetOffsetDist(offsetDist);

		/// > Phero Level 
		int level = 3;// PheroDropInfo::Min_Phero_Level + std::rand() % PheroDropInfo::Max_Phero_Level; // 1 2 3 
		script->SetLevel(level);
		mPheros.push_back(phero);

		/// > Store Phero states (All) "11231323123" = [1 lv]-(0) [1 lv]-(1) [2 lv]-(2)  ... [3 lv]
		AllPherosState += std::to_string(level);
	}


	auto type = GetOwner()->GetType();
	if (type == GameObjectInfo::Type::Monster_Onyscidus || 
		type == GameObjectInfo::Type::Monster_AdvancedCombat_5 || 
		type == GameObjectInfo::Type::Monster_Anglerox || 
		type == GameObjectInfo::Type::Monster_Arack || 
		type == GameObjectInfo::Type::Monster_Aranobot || 
		type == GameObjectInfo::Type::Monster_Ceratoferox || 
		type == GameObjectInfo::Type::Monster_Gobbler || 
		type == GameObjectInfo::Type::Monster_LightBipedMech || 
		type == GameObjectInfo::Type::Monster_MiningMech || 
		type == GameObjectInfo::Type::Monster_Onyscidus || 
		type == GameObjectInfo::Type::Monster_Rapax || 
		type == GameObjectInfo::Type::Monster_Ursacetus
		) 
	{
		/// > Set Pheros States ( All Pheros Info )
		SPtr<GameMonster> monster = std::dynamic_pointer_cast<GameMonster>(GetOwner());
		monster->SetPheros(AllPherosState);
	}

}

void Script_PheroDropper::OnCollisionWithPlayer(int PlayerID)
{
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
		Vec3 destPos = monsterDeadPoint + mPheros[i]->GetOffsetDist();
		mPheros[i]->GetTransform()->SetPosition(destPos);

	}
}
