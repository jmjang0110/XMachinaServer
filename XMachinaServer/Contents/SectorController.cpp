#include "pch.h"
#include "SectorController.h"
#include "ServerLib/MemoryManager.h"


SectorController::SectorController()
{
}

SectorController::~SectorController()
{

    for (UINT8 i = 0; i < SectorInfo::height; ++i) {
        for (UINT8 k = 0; k < SectorInfo::Width; ++k) {
            SAFE_DELETE(mSectors[i][k]);
        }
    }

}


bool SectorController::Init(Coordinate sectorTotalSize, SPtr_GameRoom owner)
{
    mOwnerRoom = owner;

    /* Memory Manager는 실시간으로 메모리를 할당할 때 쓴다 (과부하를 줄이기 위해서) 이는 처음 서버가 시작할 때 초기화 되므로 Memory Pool에서의 메모리를 쓸 필요가 없다.*/
    for (UINT8 i = 0; i < SectorInfo::height; ++i) {
        for (UINT8 k = 0; k < SectorInfo::Width; ++k) {
            mSectors[i][k] = new Sector();
            mSectors[i][k]->Init();
        }
    }

    mTotalSectorSize.x = sectorTotalSize.x;
    mTotalSectorSize.z = sectorTotalSize.z;

    mSectorSize.x = mTotalSectorSize.x / SectorInfo::Width;
    mSectorSize.z = mTotalSectorSize.z / SectorInfo::height;


    return true;
}

Vec3 SectorController::GetSectorIdx(Vec3 Pos)
{
    Vec3 sectorIdx;
    sectorIdx.x = static_cast<int>(Pos.x / mSectorSize.x);
    sectorIdx.y = 0.f;
    sectorIdx.z = static_cast<int>(Pos.y / mSectorSize.z);
    return sectorIdx;


}
