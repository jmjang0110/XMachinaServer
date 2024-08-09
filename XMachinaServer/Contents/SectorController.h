#pragma once

#include "TileMap.h"

/// +-------------------------------
///		   SectorController
/// ________________________________
/// > [ ���� ]
/// - ��ü Sector �� �����Ѵ�. �� ��ü ���� �����Ѵ�. 
/// ________________________________
/// 24.06.22 : Room�ȿ� �ִ� Session(Player)�鿡�� Packet �� ������.  ---> (���濹��) Room �ȿ� SectorManager �� ���ؼ� ���� Sector �� �ִ� Player�� �������� ������ Packet �� ���� �� 
/// 24.06.26 : SectorManager -> SectorController �� ����				---> GameManager -> Room[] -> �� Room �� SectorController�� ������ �ְ� Sector���� �����Ѵ�. 
/// ________________________________
///	> [ ���� ]
/// [ ] : Sector 
///	col(0) col(1)  
///   ��    ��
///  [	][	][	][	][	][	]...[  ] �� row(0)
///  [	][	][	][	][	][	]...[  ] �� row(1)
///  [	][	][	][	][	][	]...[  ]
///  [	][	][	][	][	][	]...[  ]
///  [	][	][	][	][	][	]...[  ]
///  [0,0][	][	][	][	][	]...[  ]
///  ��
///  Start Pos 
/// -------------------------------+

#include "GamePlayer.h"

class Sector;

/* Height map (W : 1000, H : 500 )*/
namespace SectorInfo {
	constexpr UINT8 Width	= 20;
	constexpr UINT8 height	= 10;

	enum class Type
	{
		None,
		Monsters,	/* Monster�� �ִ� Sector  */
		Boss,		/* Boss �� �����ϴ� Sector */
		End,
	};

}

class SectorController
{
private:
	SPtr_GameRoom								 mOwnerRoom; // �ڽ��� �����ִ� Room ������ 
	
private:
	std::unordered_map<UINT32, SPtr<GamePlayer>> mPlayers;
	Lock::SRWLock								 mPlayers_SRWLock;

	std::array<std::array<Sector*, SectorInfo::Width>, SectorInfo::height> mSectors;

	Coordinate									mTotalSectorSize = {}; // Sector ��ü ũ��  ( Image )
	Coordinate									mSectorSize      = {}; // �� Sector ũ��	  
	Coordinate									mSectorStartPos  = {0,0};

public:
	static Coordinate							Total_SectorSize; // Sector ��ü ũ��  ( Image )
	static Coordinate							Each_SectorSize; // �� Sector ũ��	  
	static Coordinate							SectorStartPos;



public:
	SectorController();
	~SectorController();

public:
	bool Init(SPtr_GameRoom owner);

	
	ViewList UpdateViewList(GamePlayer* player, Vec3 player_pos, float viewRange_radius);
	ViewList GetViewList(Vec3 pos, float viewRange_radius, bool DoActivate);



	/// +------------------------------------------------------------
	///		GET
	/// -------------------------------------------------------------+
	Coordinate	     GetSectorIdx(Vec3 Pos); // Position �� ���� Index �� �޴´� .
	Coordinate	     GetTotalSectorSize() 	{ return mTotalSectorSize; }
	Coordinate	     GetSectorSize()		{ return mSectorSize; }
	Coordinate	     GetMaxSectorIndex()	{ return Coordinate(static_cast<int>(mSectors[0].size()), static_cast<int>(mSectors.size())); }
	SectorInfo::Type GetSectorType(Coordinate idx);
	SPtr<GameRoom>   GetOwnerRoom()			{ return mOwnerRoom; }
	static Coordinate		GetSectorStartPos(Coordinate sectorIdx);


	/* Function */
	bool AddMonsterInSector(Coordinate sectorIdx, int monster_id , SPtr<GameMonster> monster);

public:
	/// +------------------------------------------------------------
	///		Collision ray 
	/// -------------------------------------------------------------+
	float CollideCheckRay_MinimumDist(Coordinate sectorIdx, const Ray& ray, GameObjectInfo::Type targetType) const;


public:
	static Coordinate GetSectorIdxByPosition(Vec3 Pos);

};

