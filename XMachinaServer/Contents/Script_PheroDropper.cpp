#include "pch.h"
#include "Script_PheroDropper.h"
#include "GameObject.h"

#include "Component.h"
#include "Transform.h"
#include "Collider.h"
#include "Gameinfo.h"
#include "GameMonster.h"

#include "Script_Phero.h"

// 12312491239182638123
// -> 123 124 912 ...
// -> 1 : Level , 23 : index 

namespace PheroDropInfo {
	std::vector<Vec3> Offsets = {   // 전역 변수 정의 및 초기화
		Vec3(3.5355f, 0.0f, 3.5355f),    // [ 0 ]
		Vec3(-4.9497f, 0.0f, 1.7365f),   // [ 1 ]
		Vec3(1.1106f, 0.0f, -4.8964f),   // [ 2 ]
		Vec3(-3.5355f, 0.0f, -3.5355f),  // [ 3 ]
		Vec3(4.9497f, 0.0f, -1.7365f),   // [ 4 ]
		Vec3(2.1213f, 0.0f, 4.5826f),    // [ 5 ]
		Vec3(-2.9398f, 0.0f, 4.0451f),   // [ 6 ]
		Vec3(4.0451f, 0.0f, -2.9398f),   // [ 7 ]
		Vec3(-1.7365f, 0.0f, -4.9497f),  // [ 8 ]
		Vec3(3.5355f, 0.0f, -3.5355f),   // [ 9 ]
		Vec3(1.7365f, 0.0f, 4.9497f),    // [ 10 ]
		Vec3(-2.1213f, 0.0f, 4.5826f),   // [ 11 ]
		Vec3(3.5355f, 0.0f, -3.5355f),   // [ 12 ]
		Vec3(-2.1213f, 0.0f, -4.5826f),  // [ 13 ]
		Vec3(2.1213f, 0.0f, -4.5826f),   // [ 14 ]
		Vec3(2.9398f, 0.0f, 4.0451f),    // [ 15 ]
		Vec3(-2.1213f, 0.0f, 4.5826f),   // [ 16 ]
		Vec3(4.0451f, 0.0f, -2.9398f),   // [ 17 ]
		Vec3(-1.7365f, 0.0f, -4.9497f),  // [ 18 ]
		Vec3(3.5355f, 0.0f, -3.5355f),   // [ 19 ]
		Vec3(1.7365f, 0.0f, 4.9497f),    // [ 20 ]
		Vec3(-2.9398f, 0.0f, 4.0451f),   // [ 21 ]
		Vec3(3.5355f, 0.0f, -3.5355f),   // [ 22 ]
		Vec3(-1.7365f, 0.0f, -4.9497f),  // [ 23 ]
		Vec3(3.5355f, 0.0f, -3.5355f),   // [ 24 ]
		Vec3(1.7365f, 0.0f, 4.9497f),    // [ 25 ]
		Vec3(-2.9398f, 0.0f, 4.0451f),   // [ 26 ]
		Vec3(3.5355f, 0.0f, -3.5355f),   // [ 27 ]
		Vec3(-2.1213f, 0.0f, -4.5826f),  // [ 28 ]
		Vec3(2.1213f, 0.0f, -4.5826f)    // [ 29 ]
	};
}


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
		SPtr<GameObject> phero = otherScript->GetPheros()[i]->Clone();

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
	std::vector<int> OffsetIndexList{};

	for (int i = 0; i < 30; ++i) {
		OffsetIndexList.push_back(i);
	}
	// 랜덤 엔진 생성
	std::random_device rd; // 하드웨어 난수 생성기
	std::mt19937 g(rd()); // 랜덤 엔진

	// 벡터의 요소를 무작위로 섞기
	std::shuffle(OffsetIndexList.begin(), OffsetIndexList.end(), g);

	std::string AllPherosState = {};
	int rand = 5 + std::rand() % (15 - 5);
	mPheros.reserve(rand);
	for (int i = 0; i < rand; ++i) {
		/// +---------------------------------------------------------------
		///							CREATE PHERO 
		/// ---------------------------------------------------------------+
		std::shared_ptr<GameObject> phero = std::make_shared<GameObject>(i);
		phero->AddComponent<Transform>(ComponentInfo::Type::Transform);
		phero->AddComponent<Collider>(ComponentInfo::Type::Collider);

		auto script = phero->AddScript<Script_Phero>(ScriptInfo::Type::Phero);
		script->SetOffsetDistIndex(OffsetIndexList[i]);

		int level = 1 + std::rand() % 2;
		script->SetLevel(level);
		script->Init();
		AllPherosState += script->GetPheroStateString();
		mPheros.push_back(phero);
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
