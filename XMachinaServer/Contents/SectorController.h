#pragma once


/// +-------------------------------
///		   SectorController
/// ________________________________
/// [ 설명 ]
/// - 전체 Sector 를 관리한다. 즉 전체 맵을 관리한다. 
/// ________________________________
/// 24.06.22 : Room안에 있는 Session(Player)들에게 Packet 을 보낸다.  ---> (변경예정) Room 안에 SectorManager 를 통해서 같은 Sector 에 있는 Player를 기준으로 나누고 Packet 을 보낼 것 
/// 24.06.26 : SectorManager -> SectorController 로 변경				---> GameManager -> Room[] -> 각 Room 은 SectorController을 가지고 있고 Sector들을 관리한다. 
/// ________________________________
///		
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

#include "Sector.h"

namespace SectorInfo {
	constexpr UINT8 Width  = 10;
	constexpr UINT8 height = 10;

}
class SectorController
{

private:
	std::array<std::array<Sector*, SectorInfo::height>, SectorInfo::Width> mSectors;

public:
	SectorController();
	~SectorController();

public:
	bool Init();



};

