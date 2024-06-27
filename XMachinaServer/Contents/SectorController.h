#pragma once


/// +-------------------------------
///		   SectorController
/// ________________________________
/// [ ���� ]
/// - ��ü Sector �� �����Ѵ�. �� ��ü ���� �����Ѵ�. 
/// ________________________________
/// 24.06.22 : Room�ȿ� �ִ� Session(Player)�鿡�� Packet �� ������.  ---> (���濹��) Room �ȿ� SectorManager �� ���ؼ� ���� Sector �� �ִ� Player�� �������� ������ Packet �� ���� �� 
/// 24.06.26 : SectorManager -> SectorController �� ����				---> GameManager -> Room[] -> �� Room �� SectorController�� ������ �ְ� Sector���� �����Ѵ�. 
/// ________________________________
///		
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

