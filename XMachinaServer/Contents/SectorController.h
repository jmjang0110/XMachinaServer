#pragma once


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

#include "Sector.h"

namespace SectorInfo {
	constexpr UINT8 Width  = 10;
	constexpr UINT8 height = 10;

}
class SectorController
{
private:
	SPtr_GameRoom mOwnerRoom; // �ڽ��� �����ִ� Room ������ 

private:
	std::array<std::array<Sector*, SectorInfo::height>, SectorInfo::Width> mSectors;

	Coordinate mTotalSectorSize = {}; // Sector ��ü ũ�� 
	Coordinate mSectorSize      = {}; // �� Sector ũ�� 

public:
	SectorController();
	~SectorController();

public:
	bool Init(Coordinate sectorTotalSize, SPtr_GameRoom owner);


	Vec3 GetSectorIdx(Vec3 Pos); // Position �� ���� Index �� �޴´� .


};

