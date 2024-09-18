#include "pch.h"
#include "SectorController.h"
#include "ServerLib/MemoryManager.h"
#include "ResourceManager.h"
#include "HeightMapImage.h"
#include "Sector.h"
#include "ViewList.h"

#include "GameRoom.h"
#include "NPCController.h"
#include "GameObject.h"
#include "Collider.h"
#include "Transform.h"
#include "PlayerController.h"

#include "Script_Building.h"
#include "Script_Player.h"


Coordinate SectorController::Total_SectorSize = {};
Coordinate SectorController::Each_SectorSize  = {};
Coordinate SectorController::SectorStartPos   = {};


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


bool SectorController::Init(SPtr<GameRoom> owner)
{
    mOwnerRoom                 = owner;

    SPtr<HeightMapImage> hmRes = RESOURCE_MGR->GetHeightMapImage();
    mTotalSectorSize.x         = hmRes->GetHeightMapWidth();
    mTotalSectorSize.z         = hmRes->GetHeightMapLength();
    mSectorSize.x              = mTotalSectorSize.x / SectorInfo::Width;
    mSectorSize.z              = mTotalSectorSize.z / SectorInfo::height;
    
    SectorController::Total_SectorSize = mTotalSectorSize;
    SectorController::Each_SectorSize  = mSectorSize;

    /// +-------------------------------------------------------------
    ///      SECTORs INIT 
    /// -------------------------------------------------------------+
    /* Memory Manager�� �ǽð����� �޸𸮸� �Ҵ��� �� ���� (�����ϸ� ���̱� ���ؼ�) �̴� ó�� ������ ������ �� �ʱ�ȭ �ǹǷ� Memory Pool������ �޸𸮸� �� �ʿ䰡 ����.*/
    for (UINT8 i = 0; i < SectorInfo::height; ++i) {
        for (UINT8 k = 0; k < SectorInfo::Width; ++k) {
            mSectors[i][k] = new Sector();
            Coordinate Index = Coordinate(k, i);
            mSectors[i][k]->Init(Index, SectorInfo::Type::Monsters); // �ϴ� ���� ���Ͱ� �ִ� ���ͷ� ���� 
            mSectors[i][k]->SetOwnerSectorController(this);
        }
    }

    /// +-------------------------------------------------------------
    ///      BUILDINGS 
    /// -------------------------------------------------------------+
    // Resource �� �ִ� Buildings�� ���Ϳ� ���� �и��Ѵ�. ( �̶� Resource�� �����͸��� ����. ) Buildings�� �����̰ų� �ϸ� �ȵ�! �ٲٴ°� ResourceMAnager ����...
    const std::vector<SPtr<GameObject>>* BuildingResources = RESOURCE_MGR->GetBattleScene()->GetBuildings();
    for (int i = 0; i < BuildingResources->size(); ++i) {
        
        SPtr<GameObject> building            = (*BuildingResources)[i]; // ���� �״�� �����´�. ( �ǹ��� Read Only ) 
        Vec3             Pos                 = building->GetTransform()->GetPosition();
        Coordinate       SectorIndex         = SectorController::GetSectorIdxByPosition(Pos);

        building->GetScriptEntity<Script_Building>()->SetSectorIdx(SectorIndex);
        mSectors[SectorIndex.z][SectorIndex.x]->AddBuilding(building->GetID(), building);
    }


    return true;
}


ViewList SectorController::UpdateViewList(GameObject* player, Vec3 player_pos, float viewRange_radius)
{
    ViewList vList;

    // Ȯ���ؾ��� Sector�� ���Ѵ�.
    std::vector<Coordinate> sectors = GetCheckSectors(player_pos, viewRange_radius);

    //LOG_MGR->Cout("Sector Size : ", mSectorSize.z, mSectorSize.x, "  -- curSectorIdx : ", " z :", curSectorIdx.z, " x : ", curSectorIdx.x, '\n');
    std::vector<SPtr<GameObject>> AllView_Monsters;
    std::vector<SPtr<GameObject>> AllView_Players;

    for (int i = 0; i < sectors.size(); ++i) {
        if (sectors[i].x == -1)
            continue;

        // �ش� ���Ϳ��� Monster �� Player ���� Ȯ���Ѵ�.
        //LOG_MGR->Cout(player->GetID(), " player : -- SECTORS IDX : ", " z :", sectors[i].z, " x : ", sectors[i].x, '\n');
        std::vector<SPtr<GameObject>>  VL_Monsters = mSectors[sectors[i].z][sectors[i].x]->GetMonstersInViewRange(player_pos, viewRange_radius);
        std::vector<SPtr<GameObject>>  VL_Players  = mOwnerRoom->GetPlayerController()->GetPlayersInViewRange(player_pos, viewRange_radius);

        for (int i = 0; i < VL_Monsters.size(); ++i) {
            AllView_Monsters.push_back(VL_Monsters[i]);
        }
        for (int i = 0; i < VL_Players.size(); ++i) {
            AllView_Players.push_back(VL_Players[i]);
        }
    }
     // LOG_MGR->Cout("--------------------------------------------\n");

    player->GetScriptEntity<Script_Player>()->UpdateViewList(AllView_Players, AllView_Monsters);

 
    return vList;
}

ViewList SectorController::GetViewList(Vec3 pos, float viewRange_radius, bool DoActivate)
{
    ViewList vList;

    // Ȯ���ؾ��� Sector�� ���Ѵ�.
    std::vector<Coordinate> sectors = GetCheckSectors(pos, viewRange_radius);

    for (int i = 0; i < sectors.size(); ++i) {
        if (sectors[i].x == -1)
            continue;

        std::vector<SPtr<GameObject>> VL_Monsters = mSectors[sectors[i].z][sectors[i].x]->GetMonstersInViewRange(pos, viewRange_radius);
        std::vector<SPtr<GameObject>>  VL_Players = mOwnerRoom->GetPlayerController()->GetPlayersInViewRange(pos, viewRange_radius);

        for (int i = 0; i < VL_Monsters.size(); ++i) {
            vList.TryInsertMonster(VL_Monsters[i]->GetID(), VL_Monsters[i], DoActivate);
        }
        for (int i = 0; i < VL_Players.size(); ++i) {
            vList.TryInsertPlayer(VL_Players[i]->GetID(), VL_Players[i]);
        }
    }

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

bool SectorController::AddMonsterInSector(Coordinate sectorIdx, int monster_id , SPtr<GameObject> monster)
{
    return mSectors[sectorIdx.z][sectorIdx.x]->AddMonster(monster_id, monster);

}

float SectorController::CollideCheckRay_MinimumDist(Coordinate sectorIdx, const Ray& ray) const
{
   return mSectors[sectorIdx.z][sectorIdx.x]->CollideCheckRay_MinimumDist(ray);
}

std::vector<SPtr<GameObject>> SectorController::GetEnemies_InRange(Coordinate sectorIdx, Vec3 center_pos, float radius)
{
    return mSectors[sectorIdx.z][sectorIdx.x]->GetEnemiesInRange(center_pos, radius);
}

bool SectorController::CollideCheck_WithBuildings(Coordinate sectorIndex, SPtr<GameObject> obj)
{
    if (sectorIndex.x < 0 || sectorIndex.x >= mTotalSectorSize.x || sectorIndex.z >= mTotalSectorSize.z)
        return false;
    bool Result = mSectors[sectorIndex.z][sectorIndex.x]->CollideCheck_WithBuildings(obj);
    return Result;
}

bool SectorController::CollideCheck_WithEnemies(Coordinate sectorIndex, SPtr<GameObject> obj)
{
    if (sectorIndex.x < 0 || sectorIndex.x >= mTotalSectorSize.x || sectorIndex.z >= mTotalSectorSize.z)
        return false;
    bool Result = mSectors[sectorIndex.z][sectorIndex.x]->CollideCheck_WithEnemies(obj);
    return Result;
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



Coordinate SectorController::GetSectorStartPos(Coordinate sectorIdx)
{
    int startX = SectorController::SectorStartPos.x + sectorIdx.x * SectorController::Each_SectorSize.x;
    int startZ = SectorController::SectorStartPos.z + sectorIdx.z * SectorController::Each_SectorSize.z;
    return Coordinate(startX, startZ);
}


std::vector<Coordinate> SectorController::GetCheckSectors(Vec3 pos, float radius)
{   
    // Ȯ���ؾ��� Sector�� ���Ѵ�.
    std::vector<Coordinate> sectors{};
    Coordinate curSectorIdx = GetSectorIdx(pos);
    sectors.push_back(curSectorIdx);

    Vec3 E = pos; E.x += radius;
    Coordinate East_SectorIdx = GetSectorIdx(E);
    if (East_SectorIdx == Coordinate())
        East_SectorIdx = curSectorIdx;

    Vec3 N = pos; N.z += radius;
    Coordinate North_SectorIdx = GetSectorIdx(N);
    if (North_SectorIdx == Coordinate())
        North_SectorIdx = curSectorIdx;

    Vec3 W = pos; W.x -= radius;
    Coordinate West_SectorIdx = GetSectorIdx(W);
    if (West_SectorIdx == Coordinate())
        West_SectorIdx = curSectorIdx;

    Vec3 S = pos; S.z -= radius;
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

    return sectors;
}