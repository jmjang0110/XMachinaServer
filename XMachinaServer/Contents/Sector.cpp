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
