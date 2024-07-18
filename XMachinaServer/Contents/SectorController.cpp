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


Coordinate SectorController::Total_SectorSize = {};
Coordinate SectorController::Each_SectorSize  = {};


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
    
    SectorController::Total_SectorSize = mTotalSectorSize;
    SectorController::Each_SectorSize  = mSectorSize;


    /* Memory Manager�� �ǽð����� �޸𸮸� �Ҵ��� �� ���� (�����ϸ� ���̱� ���ؼ�) �̴� ó�� ������ ������ �� �ʱ�ȭ �ǹǷ� Memory Pool������ �޸𸮸� �� �ʿ䰡 ����.*/
    for (UINT8 i = 0; i < SectorInfo::height; ++i) {
        for (UINT8 k = 0; k < SectorInfo::Width; ++k) {
            mSectors[i][k] = new Sector();
            mSectors[i][k]->Init(SectorInfo::Type::Monsters); // �ϴ� ���� ���Ͱ� �ִ� ���ͷ� ���� 
            mSectors[i][k]->SetOwnerSectorController(this);
        }
    }




    return true;
}

void SectorController::UpdateSectorsActivate(Vec3 player_pos, float radius)
{
    // �Ź� ���͸� Activate�� �ʿ䰡 �ֳ�? �׳� View List �ȿ� �ִ� �͸� ������Ʈ ����
    // �Ʒ� �ڵ�� �ϴ� ���ܵΰڴ�.. 
    std::vector<Coordinate> playerSectors{};

    Coordinate curSectorIdx = GetSectorIdx(player_pos); 
    playerSectors.push_back(curSectorIdx);

    /// ---------------------------------------
    /// [if(��On/��On)] [ �� ] [if(��On/��On)]
    /// [ �� ]          curidx          [ �� ]
    /// [if(��On/��On]] [ �� ] [if(��On/��On)]   
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

    // Ȯ���ؾ��� Sector�� ���Ѵ�.
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
    /// [if(��On/��On)] [ �� ] [if(��On/��On)]
    /// [ �� ]          curidx          [ �� ]
    /// [if(��On/��On]] [ �� ] [if(��On/��On)]   
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

        // �ش� ���Ϳ��� Monster �� Player ���� Ȯ���Ѵ�.
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

float SectorController::CheckCollisionsRay(Coordinate sectorIdx, const Ray& ray) const
{
   // return mSectors[sectorIdx.z][sectorIdx.x]->CheckCollisionsRay(ray);
    return 0.f;
}

Coordinate SectorController::GetSectorIdxByPosition(Vec3 Pos)
{
    if (Pos.x < 0 || Pos.x >= SectorController::Total_SectorSize.x || Pos.z < 0 || Pos.z >= SectorController::Total_SectorSize.z) {
        return Coordinate();
    }

    Coordinate sectorIdx{};
    sectorIdx.x = static_cast<int>((Pos.x / SectorController::Each_SectorSize.x));
    sectorIdx.z = static_cast<int>((Pos.z / SectorController::Each_SectorSize.z));
    return sectorIdx;
}

