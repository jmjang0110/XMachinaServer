#pragma once

#include "GameObject.h"
#include "SectorController.h"
/// +-------------------------------
///		        Sector
/// ________________________________
///
/// ������ ���� Sector �� ������.
/// Sector Controller �� �̸� �����Ѵ�.
/// -------------------------------+

class Sector : public GameObject
{
private:
	SectorInfo::Type mType = SectorInfo::Type::None;
	Coordinate		 mIndex = {};
	SectorController* mOwnerSC = nullptr;

private:

	std::unordered_map<UINT32, SPtr<GameObject>>		 mMonsters{};
	std::unordered_map<UINT32, SPtr<GameObject>>	     mNPCs{};
	std::unordered_map<UINT32, SPtr<GameObject>>		 mBuildings{};

public:
	Sector() = default;
	Sector(UINT32 id, Coordinate sectorIdx); 
	virtual ~Sector();

public:
	void PrintInfo();

public:
	void Init(Coordinate Index, SectorInfo::Type type = SectorInfo::Type::None);

	bool AddMonster(UINT32 id, SPtr<GameObject> monster);
	bool AddNPC(UINT32 id, SPtr<GameObject> npc);
	bool AddBuilding(UINT32 id, SPtr<GameObject> building);

public:
	/// ----------------------- Set -----------------------
	void SetOwnerSectorController(SectorController* ownerSC) { mOwnerSC = ownerSC; }

	/// ----------------------- Get -----------------------
	SectorInfo::Type				GetSectorType()					{ return mType; }
	SectorController*				GetOwnerSectorController()		{ return mOwnerSC; }
	std::vector<SPtr<GameObject>>	GetMonstersInViewRange(Vec3 player_pos, float viewRange_radius);

	/// ----------------------- Collide -----------------------
	float CollideCheckRay_MinimumDist(const Ray& ray);
	bool  CollideCheck_WithBuildings(ColliderSnapShot& other);


};

