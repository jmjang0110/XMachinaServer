#include "pch.h"
#include "TileMap.h"
#include "Sector.h"
#include "Contents/Collider.h"

int TileMap::mTileRows = 0;
int TileMap::mTileCols = 0;

TileMap::TileMap(SectorController* owner, int index, int width, int height, const DirectX::BoundingBox& bb)
{
	mOwner	  = owner;
	mIndex    = index;
	mTileRows = static_cast<int>(height / mkTileHeight);
	mTileCols = static_cast<int>(width / mkTileWidth);
	mTiles    = std::vector<std::vector<TileMapInfo::TileType>>(mTileRows, std::vector<TileMapInfo::TileType>(mTileCols, TileMapInfo::TileType::None));

}

Path::Pos TileMap::GetTileUniqueIndexFromPos(const Vec3& pos) const
{
	const int tileGroupIndexX = static_cast<int>((pos.x - mTileMapStartPoint) / TileMap::mkTileWidth);
	const int tileGroupIndexZ = static_cast<int>((pos.z - mTileMapStartPoint) / TileMap::mkTileHeight);

	return Path::Pos{ tileGroupIndexZ, tileGroupIndexX };
}

Vec3 TileMap::GetTilePosFromUniqueIndex(const Path::Pos& index) const
{
	const float posX = index.X * TileMap::mkTileWidth + mTileMapStartPoint;
	const float posZ = index.Z * TileMap::mkTileHeight + mTileMapStartPoint;

	return Vec3{ posX, 0, posZ };
}

TileMapInfo::TileType TileMap::GetTileFromUniqueIndex(const Path::Pos& index) const
{
	return mTiles[index.Z][index.X];
}

void TileMap::SetTileFromUniqueIndex(const Path::Pos& index, TileMapInfo::TileType tile)
{
	mTiles[index.Z][index.X] = tile;
}

TileMapInfo::TileType TileMap::GetTileFromPos(const Vec3& pos) const
{
	return GetTileFromUniqueIndex(GetTileUniqueIndexFromPos(pos));
}

void TileMap::UpdateTiles(TileMapInfo::TileType tile, GameObject* object)
{
	// 정적 오브젝트가 Building 태그인 경우에만 벽으로 설정
	if (object->GetType() != GameObjectInfo::Type::Building)
		return;

	// BFS를 통해 주변 타일도 업데이트
	std::queue<Path::Pos> q;
	std::map<Path::Pos, bool> visited;

	// 오브젝트의 충돌 박스
	for (const auto& collider : object->GetComponent<ObjectCollider>(ComponentInfo::Type::ObjectCollider)->GetColliders()) {
		if (collider->GetType() != Collider::Type::Box) {
			continue;
		}



		// 오브젝트의 타일 기준 인덱스 계산
		Vec3 pos = collider->GetCenter();
		Path::Pos index = GetTileUniqueIndexFromPos(pos);
		SetTileFromUniqueIndex(index, tile);
		q.push(index);

		// q가 빌 때까지 BFS를 돌며 현재 타일이 오브젝트와 충돌 했다면 해당 타일을 업데이트
		while (!q.empty()) {
     	Path::Pos curNode = q.front();
			q.pop();

			if (visited[curNode])
				continue;

			visited[curNode] = true;

			for (int dir = 0; dir < 4; ++dir) {
				Path::Pos nextPosT = curNode + Path::gkFront[dir];
				if (nextPosT.X < 0 || nextPosT.Z < 0) {
					continue;
				}

				Vec3 nextPosW = GetTilePosFromUniqueIndex(nextPosT);
				nextPosW.y = pos.y;

				BoundingBox bb{ nextPosW, Vec3{mkTileWidth, mkTileWidth, mkTileHeight} };

				if (collider->Intersects(bb)) {
					SetTileFromUniqueIndex(nextPosT, tile);
					q.push(nextPosT);
				}
			}
		}
	}
}
