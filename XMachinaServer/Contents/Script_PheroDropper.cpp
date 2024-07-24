#include "pch.h"
#include "Script_PheroDropper.h"
#include "GameObject.h"

#include "Component.h"
#include "Transform.h"
#include "Collider.h"
#include "Gameinfo.h"
#include "GameMonster.h"

#include "Script_Phero.h"

namespace PheroDropInfo {
	std::vector<Vec3> Offsets = {   // 전역 변수 정의 및 초기화
		Vec3(8.4362f, 0.0f, 14.8971f),    // [ 0 ]
		Vec3(-13.5674f, 0.0f, 5.2095f),   // [ 1 ]
		Vec3(3.7562f, 0.0f, -16.2642f),   // [ 2 ]
		Vec3(-15.8941f, 0.0f, -3.3461f),  // [ 3 ]
		Vec3(11.4721f, 0.0f, -11.4721f),  // [ 4 ]
		Vec3(4.5678f, 0.0f, 17.8923f),    // [ 5 ]
		Vec3(-7.2345f, 0.0f, 18.3456f),   // [ 6 ]
		Vec3(12.9876f, 0.0f, -8.6543f),   // [ 7 ]
		Vec3(-6.4321f, 0.0f, -18.7654f),  // [ 8 ]
		Vec3(15.6789f, 0.0f, -9.3456f),   // [ 9 ]
		Vec3(9.8765f, 0.0f, 11.4321f),    // [ 10 ]
		Vec3(-8.1234f, 0.0f, 15.6789f),   // [ 11 ]
		Vec3(13.8765f, 0.0f, -5.4321f),   // [ 12 ]
		Vec3(-10.6543f, 0.0f, -14.8765f), // [ 13 ]
		Vec3(7.2345f, 0.0f, -15.3456f),   // [ 14 ]
		Vec3(5.4321f, 0.0f, 19.1234f),    // [ 15 ]
		Vec3(-9.8765f, 0.0f, 13.5678f),   // [ 16 ]
		Vec3(14.6543f, 0.0f, -10.8765f),  // [ 17 ]
		Vec3(-5.6789f, 0.0f, -17.4321f),  // [ 18 ]
		Vec3(10.4321f, 0.0f, -14.5678f),  // [ 19 ]
		Vec3(6.7890f, 0.0f, 16.1234f),    // [ 20 ]
		Vec3(-11.8765f, 0.0f, 10.8765f),  // [ 21 ]
		Vec3(12.3456f, 0.0f, -12.3456f),  // [ 22 ]
		Vec3(-8.9876f, 0.0f, -12.9876f),  // [ 23 ]
		Vec3(9.1234f, 0.0f, -14.1234f),   // [ 24 ]
		Vec3(7.6543f, 0.0f, 17.6543f),    // [ 25 ]
		Vec3(-6.7890f, 0.0f, 11.7890f),   // [ 26 ]
		Vec3(11.1234f, 0.0f, -10.1234f),  // [ 27 ]
		Vec3(-10.5678f, 0.0f, -10.5678f), // [ 28 ]
		Vec3(8.1234f, 0.0f, -11.1234f)    // [ 29 ]
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

	for (int i = 0; i < otherScript->getPheros().size(); ++i) {
		SPtr<GameObject> phero = otherScript->getPheros()[i]->Clone();

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
	return std::round(totalNumber * (percentage / 100.0));
}

void Script_PheroDropper::Shuffle_OdffsetDistIndexList()
{

}
