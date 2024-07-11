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
	SPtr_GameRoom mOwnerRoom; // �ڽ��� �����ִ� Room ������ 

private:
	std::array<std::array<Sector*, SectorInfo::height>, SectorInfo::Width> mSectors;

	Coordinate mTotalSectorSize = {}; // Sector ��ü ũ��  ( Image )
	Coordinate mSectorSize      = {}; // �� Sector ũ��	  ( Image )

public:
	SectorController();
	~SectorController();

public:
	bool Init(SPtr_GameRoom owner);


	Vec3 GetSectorIdx(Vec3 Pos); // Position �� ���� Index �� �޴´� .
	Coordinate GetTotalSectorSize() { return mTotalSectorSize; }
	Coordinate GetSectorSize()		{ return mSectorSize; }
	Coordinate GetMaxSectorIndex() { return Coordinate(static_cast<int>(mSectors[0].size()), static_cast<int>(mSectors.size())); }

	SectorInfo::Type GetSectorType(Coordinate idx);

};

