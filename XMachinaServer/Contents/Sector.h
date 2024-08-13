#pragma once

#include "SectorController.h"
#include "GameObject.h"
#include "GamePlayer.h"
#include "GameMonster.h"
#include "GameNPC.h"

/// +-------------------------------
///		        Sector
/// ________________________________
///
/// 지형을 여러 Sector 로 나눈다.
/// Sector Controller 가 이를 관리한다.
/// 
/// 24.06.22 : 어떤 Player 들이 들어와 있는지 관리하자.!! 
/// 24.06.27 : Player , NPC - 어떤 ID 가 해당 Sector에 속해있는지 관리한다. 실제 객체는 PlayerController 나 NPCController에 있으니 여기에 있는 ID를 통해서 접근한다. 
/// -------------------------------+

class GameMonster;
class GameNPC;
class SectorController;

class Sector : public GameObject
{
private:
	SectorInfo::Type mType = SectorInfo::Type::None;
	Coordinate		 mIndex = {};
	SectorController* mOwnerSC = nullptr;

private:
	std::atomic<int> mActivateRef = 0;

	std::unordered_map<UINT32, SPtr<GameMonster>>		 mMonsters;
	std::unordered_map<UINT32, SPtr<GameNPC>>			 mNPCs;
	std::unordered_map<UINT32, SPtr<GameObject>>		 mBuildings; // 구조물 ( ex. 빌딩.. ) 

public:
	Sector();
	Sector(UINT32 id, Coordinate sectorIdx); /* NPC 생성 아이디 - (생성되고 소멸될 때 까지 임시 아이디)*/
	virtual ~Sector();

public:
	void PrintInfo();


public:
	virtual void Activate();
	virtual void DeActivate();
	virtual bool IsActive();


	virtual void Update() override;
	virtual void WakeUp() override;
	virtual void Start();

	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;
	/* 초기화 - Room/NPCController가 Sector에 Monster를 추가한다. */
public:
	void Init(Coordinate Index, SectorInfo::Type type = SectorInfo::Type::None);

	bool AddMonster(UINT32 id, SPtr<GameMonster> monster);
	bool AddNPC(UINT32 id, SPtr<GameNPC> npc);
	bool AddBuilding(UINT32 id, SPtr<GameObject> building);

	void RemoveMonster(UINT32 id);
	
public:
	/// +---------------------------------------------------------
	///	S E T T E R
	/// ---------------------------------------------------------+
	void SetOwnerSectorController(SectorController* ownerSC) { mOwnerSC = ownerSC; }


	/// +---------------------------------------------------------
	///	G E T T E R
	/// ---------------------------------------------------------+
	SectorInfo::Type				GetSectorType()					{ return mType; }
	SectorController*				GetOwnerSectorController()		{ return mOwnerSC; }
	std::vector<SPtr<GameMonster>>	GetMonstersInViewRange(Vec3 player_pos, float viewRange_radius);
	SPtr<GameObject>				GetBuilding(UINT32 id);


	/// +---------------------------------------------------------
	///	  C O L L I S I O N 
	/// ---------------------------------------------------------+
	float CollideCheckRay_MinimumDist(const Ray& ray, GameObjectInfo::Type targetType) ;
	bool  CollideCheck_WithBuildings(ColliderSnapShot& other);


};

