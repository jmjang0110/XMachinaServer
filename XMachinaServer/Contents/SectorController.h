#pragma once


/// +-------------------------------
///		   SectorController
/// ________________________________
/// > [ 설명 ]
/// - 전체 Sector 를 관리한다. 즉 전체 맵을 관리한다. 
/// ________________________________
/// 24.06.22 : Room안에 있는 Session(Player)들에게 Packet 을 보낸다.  ---> (변경예정) Room 안에 SectorManager 를 통해서 같은 Sector 에 있는 Player를 기준으로 나누고 Packet 을 보낼 것 
/// 24.06.26 : SectorManager -> SectorController 로 변경				---> GameManager -> Room[] -> 각 Room 은 SectorController을 가지고 있고 Sector들을 관리한다. 
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
		Monsters,	/* Monster가 있는 Sector  */
		Boss,		/* Boss 가 존재하는 Sector */
		End,
	};

}
class SectorController
{
private:
	SPtr_GameRoom mOwnerRoom; // 자신이 속해있는 Room 포인터 

private:
	std::unordered_map<UINT32, SPtr<GamePlayer>> mPlayers;
	Lock::SRWLock mPlayers_SRWLock;

	std::array<std::array<Sector*, SectorInfo::height>, SectorInfo::Width> mSectors;

	Coordinate mTotalSectorSize = {}; // Sector 전체 크기  ( Image )
	Coordinate mSectorSize      = {}; // 각 Sector 크기	  

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
	Coordinate	     GetSectorIdx(Vec3 Pos); // Position 에 따른 Index 를 받는다 .
	Coordinate	     GetTotalSectorSize() 	{ return mTotalSectorSize; }
	Coordinate	     GetSectorSize()		{ return mSectorSize; }
	Coordinate	     GetMaxSectorIndex()	{ return Coordinate(static_cast<int>(mSectors[0].size()), static_cast<int>(mSectors.size())); }
	SectorInfo::Type GetSectorType(Coordinate idx);
	SPtr<GameRoom>   GetOwnerRoom()			{ return mOwnerRoom; }

	/* Function */
	bool AddMonsterInSector(Coordinate sectorIdx, int monster_id , SPtr<GameMonster> monster);

	
};

