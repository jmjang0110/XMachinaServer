#include "pch.h"
#include "Sector.h"
#include "GameObject.h"
#include "GameBuilding.h"
#include "Collider.h"
#include "CollisionManager.h"


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

void Sector::PrintInfo()
{
	LOG_MGR->Cout("-------------------------------------------------\n");
	LOG_MGR->Cout("SECTOR ( ", mIndex.x, ", ", mIndex.z, " ) \n");

	if (SectorInfo::Type::Monsters == mType)
		LOG_MGR->Cout("MONSTERS TYPE\n");
	
	if (SectorInfo::Type::Boss == mType)
		LOG_MGR->Cout("BOSS TYPE\n");
	
	if (SectorInfo::Type::None == mType)
		LOG_MGR->Cout("None TYPE\n");
	
	LOG_MGR->Cout("INFO - NPCS : ", mNPCs.size(), " Buildings : ", mBuildings.size(), " Monsters : ", mMonsters.size(), "\n");



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

void Sector::Init(Coordinate Index, SectorInfo::Type type)
{
	mType  = type;
	mIndex = Index;
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

bool Sector::AddBuilding(UINT32 id, SPtr<GameObject> building)
{
	auto it = mBuildings.find(id);
	if (it == mBuildings.end()) {
		mBuildings.insert(std::make_pair(id, building));
		return true;
	}
	return false;
}

std::vector<SPtr<GameMonster>> Sector::GetMonstersInViewRange(Vec3 player_pos, float viewRange_radius)
{
	std::vector<SPtr<GameMonster>> monstersInView;
	for (auto& Mon : mMonsters) {
		Vec3 pos = Mon.second->GetTransform()->GetSnapShot().GetPosition(); /* Snap Shot - Position */

		// x, z ��ǥ ���� �Ÿ� ���
		float distance = static_cast<float>(std::sqrt(std::pow(pos.x - player_pos.x, 2) + std::pow(pos.z - player_pos.z, 2)));

		// �Ÿ� ��
		if (distance <= viewRange_radius) {
			monstersInView.push_back(Mon.second);
		}
	}

	return monstersInView;
}



float Sector::CollideCheckRay_MinimumDist(const Ray& ray, GameObjectInfo::Type targetType)
{
	float minDist = 999.f;
	float Result;

	for (auto& [ID, object] : mBuildings) {
		const ColliderSnapShot snapShot = mBuildings[ID]->GetCollider()->GetSnapShot();
		Ray R = ray;
		Result = COLLISION_MGR->CollideCheckRay_MinimumDist(snapShot, R);
		minDist = min(Result, minDist);

	}
	return minDist;
}

bool Sector::CollideCheck_WithBuildings(ColliderSnapShot& other)
{
	bool Result = false;
	for (auto& iter : mBuildings) {
		ColliderSnapShot col = iter.second->GetCollider()->GetColliderSnapShot();

		Result = COLLISION_MGR->CollideCheck_Sphere_Box(other, col);
		if (Result)
			return true;
	}
	
	return false;
}


