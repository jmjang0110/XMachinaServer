#include "pch.h"
#include "SectorController.h"
#include "ServerLib/MemoryManager.h"
#include "ResourceManager.h"
#include "HeightMapImage.h"
#include "Sector.h"

#include "GameRoom.h"
#include "NPCController.h"
#include "GameMonster.h"
#include "GameObject.h"
#include "Transform.h"
#include "PlayerController.h"


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


bool SectorController::Init(SPtr_GameRoom owner)
{
    mOwnerRoom                 = owner;

    SPtr<HeightMapImage> hmRes = RESOURCE_MGR->GetHeightMapImage();
    mTotalSectorSize.x         = hmRes->GetHeightMapWidth();
    mTotalSectorSize.z         = hmRes->GetHeightMapLength();
    mSectorSize.x              = mTotalSectorSize.x / SectorInfo::Width;
    mSectorSize.z              = mTotalSectorSize.z / SectorInfo::height;

    /* Memory Manager는 실시간으로 메모리를 할당할 때 쓴다 (과부하를 줄이기 위해서) 이는 처음 서버가 시작할 때 초기화 되므로 Memory Pool에서의 메모리를 쓸 필요가 없다.*/
    for (UINT8 i = 0; i < SectorInfo::height; ++i) {
        for (UINT8 k = 0; k < SectorInfo::Width; ++k) {
            mSectors[i][k] = new Sector();
            mSectors[i][k]->Init(SectorInfo::Type::Monsters); // 일단 전부 몬스터가 있는 섹터로 지정 
            mSectors[i][k]->SetOwnerSectorController(this);
        }
    }




    return true;
}

void SectorController::UpdateSectorsActivate(Vec3 player_pos, float radius)
{
    // 매번 섹터를 Activate할 필요가 있나? 그냥 View List 안에 있는 것만 업데이트 하자
    // 아래 코드는 일단 냄겨두겠다.. 
    std::vector<Coordinate> playerSectors{};

    Coordinate curSectorIdx = GetSectorIdx(player_pos); 
    playerSectors.push_back(curSectorIdx);

    /// ---------------------------------------
    /// [if(북On/서On)] [ 북 ] [if(북On/동On)]
    /// [ 서 ]          curidx          [ 동 ]
    /// [if(서On/남On]] [ 남 ] [if(동On/남On)]   
    /// ______________________________________
    Vec3 E                     = player_pos; E.x += radius;
    Coordinate East_SectorIdx  = GetSectorIdx(E);
    Vec3 N                     = player_pos; N.y += radius;
    Coordinate North_SectorIdx = GetSectorIdx(N);
    Vec3 W                     = player_pos; W.x -= radius;
    Coordinate West_SectorIdx  = GetSectorIdx(W);
    Vec3 S                     = player_pos; S.y -= radius;
    Coordinate South_SectorIdx = GetSectorIdx(S);

    bool E_check = curSectorIdx != East_SectorIdx;
    bool N_check = curSectorIdx != North_SectorIdx;
    bool W_check = curSectorIdx != West_SectorIdx;
    bool S_check = curSectorIdx != South_SectorIdx;

    if (E_check)
        mSectors[East_SectorIdx.z][East_SectorIdx.x]->Activate();   // Ref ++
    if (N_check)
        mSectors[North_SectorIdx.z][North_SectorIdx.x]->Activate(); // Ref ++
    if (W_check)
        mSectors[West_SectorIdx.z][West_SectorIdx.x]->Activate();   // Ref ++
    if (S_check)
        mSectors[South_SectorIdx.z][South_SectorIdx.x]->Activate(); // Ref ++
    
    if (E_check && N_check)
        mSectors[North_SectorIdx.z][East_SectorIdx.x]->Activate();  // Ref ++
    if (W_check && N_check)                                         
        mSectors[North_SectorIdx.z][West_SectorIdx.x]->Activate();  // Ref ++
    if (W_check && S_check)                                         
        mSectors[South_SectorIdx.z][West_SectorIdx.x]->Activate();  // Ref ++
    if (E_check && S_check)                                         
        mSectors[South_SectorIdx.z][East_SectorIdx.x]->Activate();  // Ref ++

    


}

ViewList SectorController::UpdateViewList(GamePlayer* player, Vec3 player_pos, float viewRange_radius)
{
    ViewList vList;

    // 확인해야할 Sector를 구한다.
    std::vector<Coordinate> sectors{};
    Coordinate curSectorIdx = GetSectorIdx(player_pos);
    sectors.push_back(curSectorIdx);

    Vec3 E = player_pos; E.x += viewRange_radius;
    Coordinate East_SectorIdx = GetSectorIdx(E);
    if (East_SectorIdx == Coordinate())
        East_SectorIdx = curSectorIdx;

    Vec3 N = player_pos; N.y += viewRange_radius;
    Coordinate North_SectorIdx = GetSectorIdx(N);
    if (North_SectorIdx == Coordinate())
        North_SectorIdx = curSectorIdx;

    Vec3 W = player_pos; W.x -= viewRange_radius;
    Coordinate West_SectorIdx = GetSectorIdx(W);
    if (West_SectorIdx == Coordinate())
        West_SectorIdx = curSectorIdx;

    Vec3 S = player_pos; S.y -= viewRange_radius;
    Coordinate South_SectorIdx = GetSectorIdx(S);
    if (South_SectorIdx == Coordinate())
        South_SectorIdx = curSectorIdx;

    /// ---------------------------------------
    /// [if(북On/서On)] [ 북 ] [if(북On/동On)]
    /// [ 서 ]          curidx          [ 동 ]
    /// [if(서On/남On]] [ 남 ] [if(동On/남On)]   
    /// ______________________________________
    
    bool E_check = curSectorIdx != East_SectorIdx;
    bool N_check = curSectorIdx != North_SectorIdx;
    bool W_check = curSectorIdx != West_SectorIdx;
    bool S_check = curSectorIdx != South_SectorIdx;

    if (E_check)              sectors.push_back(East_SectorIdx);
    if (N_check)              sectors.push_back(North_SectorIdx);
    if (W_check)              sectors.push_back(West_SectorIdx);
    if (S_check)              sectors.push_back(South_SectorIdx);
    if (E_check && N_check)   sectors.push_back(Coordinate(East_SectorIdx.x, North_SectorIdx.z)); 
    if (W_check && N_check)   sectors.push_back(Coordinate(West_SectorIdx.x, North_SectorIdx.z)); 
    if (W_check && S_check)   sectors.push_back(Coordinate(West_SectorIdx.x, South_SectorIdx.z)); 
    if (E_check && S_check)   sectors.push_back(Coordinate(East_SectorIdx.x, South_SectorIdx.z)); 


    //LOG_MGR->Cout("Sector Size : ", mSectorSize.z, mSectorSize.x, "  -- curSectorIdx : ", " z :", curSectorIdx.z, " x : ", curSectorIdx.x, '\n');
    std::vector<SPtr<GameMonster>> AllView_Monsters;
    std::vector<SPtr<GamePlayer>> AllView_Players;

    for (int i = 0; i < sectors.size(); ++i) {
        if (sectors[i].x == -1)
            continue;

        // 해당 섹터에서 Monster 와 Player 들을 확인한다.
        //LOG_MGR->Cout("Sector Size : ", mSectorSize.z, mSectorSize.x , "  -- SECTORS IDX : ", " z :", sectors[i].z, " x : ", sectors[i].x, '\n');
        std::vector<SPtr<GameMonster>> VL_Monsters = mSectors[sectors[i].z][sectors[i].x]->GetMonstersInViewRange(player_pos, viewRange_radius);
        std::vector<SPtr<GamePlayer>>  VL_Players  = mOwnerRoom->GetPlayerController()->GetPlayersInViewRange(player_pos, viewRange_radius);

        for (int i = 0; i < VL_Monsters.size(); ++i) {
            AllView_Monsters.push_back(VL_Monsters[i]);
        }
        for (int i = 0; i < VL_Players.size(); ++i) {
            AllView_Players.push_back(VL_Players[i]);
        }
    }

    player->UpdateViewList(AllView_Players, AllView_Monsters);

 
    return vList;
}

Coordinate SectorController::GetSectorIdx(Vec3 Pos)
{
    if (Pos.x < 0 || Pos.x >= mTotalSectorSize.x|| Pos.z < 0 || Pos.z >= mTotalSectorSize.z) {
        return Coordinate();
    }

    Coordinate sectorIdx{};
    sectorIdx.x = static_cast<int>((Pos.x / mSectorSize.x));
    sectorIdx.z = static_cast<int>((Pos.z / mSectorSize.z));
    return sectorIdx;


}

SectorInfo::Type SectorController::GetSectorType(Coordinate idx)
{
    return mSectors[idx.z][idx.x]->GetSectorType();
}

bool SectorController::AddMonsterInSector(Coordinate sectorIdx, int monster_id , SPtr<GameMonster> monster)
{
    return mSectors[sectorIdx.z][sectorIdx.x]->AddMonster(monster_id, monster);

}
