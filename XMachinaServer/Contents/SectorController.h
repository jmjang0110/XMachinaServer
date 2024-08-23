#pragma once

#include "TileMap.h"

/// +-------------------------------
///		   SectorController
/// ________________________________
/// > [ 설명 ]
/// - 전체 Sector 를 관리한다. 즉 전체 맵을 관리한다. 
/// ________________________________
///	> [ 구조 ]
/// [ ] : Sector 
///	col(0) col(1)  
///   ↓    ↓
///  [	][	][	][	][	][	]...[  ] ← row(0)
///  [	][	][	][	][	][	]...[  ] ← row(1)
///  [	][	][	][	][	][	]...[  ]
///  [	][	][	][	][	][	]...[  ]
///  [	][	][	][	][	][	]...[  ]
///  [0,0][	][	][	][	][	]...[  ]
///  ↑
///  Start Pos 
/// -------------------------------+


/* Height map (W : 1000, H : 500 )*/
namespace SectorInfo {
	constexpr UINT8 Width	= 20;
	constexpr UINT8 height	= 10;

	enum class Type
	{
		None,
		Monsters,	/* Monster가 있는 Sector  */
		Boss,		/* Boss 가 존재하는 Sector */
		End,
	};

}

class	Sector;
class	ViewList;
class	GameRoom;
struct	ColliderSnapShot;

class SectorController
{
private:
	SPtr<GameRoom>								 mOwnerRoom; // 자신이 속해있는 Room 포인터 
	
private:
	std::unordered_map<UINT32, SPtr<GameObject>> mPlayers;
	Lock::SRWLock								 mPlayers_SRWLock;

	std::array<std::array<Sector*, SectorInfo::Width>, SectorInfo::height> mSectors;

	Coordinate									mTotalSectorSize = {}; // Sector 전체 크기  ( Image )
	Coordinate									mSectorSize      = {}; // 각 Sector 크기	  
	Coordinate									mSectorStartPos  = {0,0};

public:
	static Coordinate							Total_SectorSize; // Sector 전체 크기  ( Image )
	static Coordinate							Each_SectorSize; // 각 Sector 크기	  
	static Coordinate							SectorStartPos;



public:
	SectorController();
	~SectorController();

public:
	bool Init(SPtr<GameRoom> owner);

	
	ViewList UpdateViewList(GameObject* player, Vec3 player_pos, float viewRange_radius);
	ViewList GetViewList(Vec3 pos, float viewRange_radius, bool DoActivate);



	/// +------------------------------------------------------------
	///		GET
	/// -------------------------------------------------------------+
	SPtr<GameRoom>			GetOwnerRoom()					{ return mOwnerRoom; }
	Coordinate				GetTotalSectorSize() 			{ return mTotalSectorSize; }
	Coordinate				GetSectorSize()					{ return mSectorSize; }
	Coordinate				GetMaxSectorIndex()				{ return Coordinate(static_cast<int>(mSectors[0].size()), static_cast<int>(mSectors.size())); }
	Coordinate				GetSectorIdx(Vec3 Pos);			// Position 에 따른 Index 를 받는다 .
	SectorInfo::Type		GetSectorType(Coordinate idx);
	static Coordinate		GetSectorStartPos(Coordinate sectorIdx);


	/* Function */
	bool AddMonsterInSector(Coordinate sectorIdx, int monster_id , SPtr<GameObject> monster);

public:
	/// +------------------------------------------------------------
	///		Collision ray 
	/// -------------------------------------------------------------+
	float CollideCheckRay_MinimumDist(Coordinate sectorIdx, const Ray& ray) const;
	bool  CollideCheck_WithBuildings(Vec3& pos, ColliderSnapShot& other);

public:
	static Coordinate GetSectorIdxByPosition(Vec3 Pos);
};

