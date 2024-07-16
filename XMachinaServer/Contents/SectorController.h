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
///  [	][	][	][	][	][	]...[  ]
/// 
/// -------------------------------+

#include "GamePlayer.h"

class Sector;

/* Height map (W : 1000, H : 500 )*/
namespace SectorInfo {
	constexpr UINT8 Width  = 10;
	constexpr UINT8 height = 10;

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
	SPtr<TileMap>									 mTileMap;

private:
	std::unordered_map<UINT32, SPtr<GamePlayer>> mPlayers;
	Lock::SRWLock								 mPlayers_SRWLock;

	std::array<std::array<Sector*, SectorInfo::height>, SectorInfo::Width> mSectors;

	Coordinate									mTotalSectorSize = {}; // Sector ��ü ũ��  ( Image )
	Coordinate									mSectorSize      = {}; // �� Sector ũ��	  

public:
	SectorController();
	~SectorController();

public:
	bool Init(SPtr_GameRoom owner);

	void UpdateSectorsActivate(Vec3 player_pos, float radius);
	
	ViewList UpdateViewList(GamePlayer* player, Vec3 player_pos, float viewRange_radius);


	/// +------------------------------------------------------------
	///		GET
	/// -------------------------------------------------------------+
	Coordinate	     GetSectorIdx(Vec3 Pos); // Position �� ���� Index �� �޴´� .
	Coordinate	     GetTotalSectorSize() 	{ return mTotalSectorSize; }
	Coordinate	     GetSectorSize()		{ return mSectorSize; }
	Coordinate	     GetMaxSectorIndex()	{ return Coordinate(static_cast<int>(mSectors[0].size()), static_cast<int>(mSectors.size())); }
	SectorInfo::Type GetSectorType(Coordinate idx);
	SPtr<GameRoom>   GetOwnerRoom()			{ return mOwnerRoom; }

	/* Function */
	bool AddMonsterInSector(Coordinate sectorIdx, int monster_id , SPtr<GameMonster> monster);

	
};

