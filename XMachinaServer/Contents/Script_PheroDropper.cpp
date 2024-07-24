#include "pch.h"
#include "Script_PheroDropper.h"
#include "GameObject.h"

#include "Component.h"
#include "Transform.h"
#include "Collider.h"
#include "Gameinfo.h"


#include "Script_Phero.h"

namespace PheroDropInfo {
	std::vector<Vec3> Offsets = {   // ���� ���� ���� �� �ʱ�ȭ
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
	for (int i = 0; i < 30; ++i) {
		mOffsetdistIndexList.push_back(i);
	}
	Shuffle_OdffsetDistIndexList();

	int rand = std::rand() % 15;
	mPheros.resize(rand);
	for (int i = 0; i < rand; ++i) {
		GameObject* phero = new GameObject;
		phero->AddComponent<Transform>(ComponentInfo::Type::Transform);
		phero->AddComponent<Collider>(ComponentInfo::Type::Collider);
		auto script = phero->AddScript<Script_Phero>(ScriptInfo::Type::Phero);

		//Vec3 a = PheroDropInfo::Offsets[0];

		//script->SetOffsetDist(PheroDropInfo::Offsets[mOffsetdistIndexList[i]]);
		
		mPheros.push_back(phero);
	}
}

Script_PheroDropper::~Script_PheroDropper()
{
}

void Script_PheroDropper::Clone(SPtr<Component> other) 
{
	Script::Clone(other);
	SPtr<Script_PheroDropper> otherScript = std::static_pointer_cast<Script_PheroDropper>(other);

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

void Script_PheroDropper::Shuffle_OdffsetDistIndexList()
{
	// ���� ���� ����
	std::random_device rd; // �ϵ���� ���� ������
	std::mt19937 g(rd()); // ���� ����

	// ������ ��Ҹ� �������� ����
	std::shuffle(mOffsetdistIndexList.begin(), mOffsetdistIndexList.end(), g);
}
