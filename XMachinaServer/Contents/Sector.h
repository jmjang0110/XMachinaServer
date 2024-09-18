#pragma once

#include "GameObject.h"
#include "SectorController.h"
/// +-------------------------------
///		        Sector
/// ________________________________
///
/// 지형을 여러 Sector 로 나눈다.
/// Sector Controller 가 이를 관리한다.
/// -------------------------------+

class Sector : public GameObject
{
private:
	SectorInfo::Type  mType = SectorInfo::Type::None;
	Coordinate		  mIndex = {};
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
	bool  CollideCheck_WithBuildings(SPtr<GameObject> obj);
	bool  CollideCheck_WithEnemies(SPtr<GameObject> obj);
	std::vector<SPtr<GameObject>> GetEnemiesInRange(Vec3 center_pos, float radius);


};

