#include "pch.h"
#include "Sector.h"



Sector::Sector()
	: GameObject(-1)
{
}

Sector::Sector(UINT32 id, Coordinate sectorIdx)
	: GameObject(id)
{
	mIndex = sectorIdx;

}


Sector::~Sector()
{
}

void Sector::Activate()
{
	if (mActivateRef.load() == 0)
		GameObject::Activate();

	mActivateRef.fetch_add(1);

}

void Sector::DeActivate()
{
	mActivateRef.fetch_sub(1);
	if (mActivateRef.load() < 0)
	{
		GameObject::DeActivate();
	}
}

bool Sector::IsActive()
{
	if (mActivateRef.load() > 0)
		return true;
	else
		return false;
}

void Sector::Update()
{
	
}

void Sector::WakeUp()
{
}

void Sector::Start()
{
}

void Sector::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{
}

void Sector::Init(SectorInfo::Type type)
{
	mType = type;

}

bool Sector::AddMonster(UINT32 id, SPtr<GameMonster> monster)
{
	auto it = mMonsters.find(id);
	if (it == mMonsters.end()) {
		mMonsters.insert(std::make_pair(id, monster));
		return true;
	}
	return false;
}

bool Sector::AddNPC(UINT32 id, SPtr<GameNPC> npc)
{
	auto it = mNPCs.find(id);
	if (it == mNPCs.end()) {
		mNPCs.insert(std::make_pair(id, npc));
		return true;
	}
	return false;
}

std::vector<SPtr<GameMonster>> Sector::GetMonstersInViewRange(Vec3 player_pos, float viewRange_radius)
{
	std::vector<SPtr<GameMonster>> monstersInView;

	for (auto& Mon : mMonsters) {
		Vec3 pos = Mon.second->GetPosition(); /* Snap Shot - Position */

		// x, z 좌표 간의 거리 계산
		float distance = static_cast<float>(std::sqrt(std::pow(pos.x - player_pos.x, 2) + std::pow(pos.z - player_pos.z, 2)));

		// 거리 비교
		if (distance <= viewRange_radius) {
			monstersInView.push_back(Mon.second);
		}
	}

	return monstersInView;
}


