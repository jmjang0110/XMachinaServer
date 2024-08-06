#include "pch.h"
#include "TileMap.h"
#include "Sector.h"
#include "Contents/Collider.h"
#include "Collider.h"

int TileMap::mTileRows = 0;
int TileMap::mTileCols = 0;

TileMap::TileMap()
{
	mTileRows = static_cast<int>(mTileMapZLength / mkTileHeight);
	mTileCols = static_cast<int>(mTileMapXLength / mkTileWidth);
	mTiles = std::vector<std::vector<TileMapInfo::TileType>>(mTileRows, std::vector<TileMapInfo::TileType>(mTileCols, TileMapInfo::TileType::None));

}


Path::Pos TileMap::GetTileUniqueIndexFromPos(const Vec3& pos) const
{
	const int tileIndex_x = static_cast<int>((pos.x - mTileMapStartPoint) / TileMap::mkTileWidth);
	const int tileIndex_z = static_cast<int>((pos.z - mTileMapStartPoint) / TileMap::mkTileHeight);

	return Path::Pos{ tileIndex_z, tileIndex_x };
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
	// ���� ������Ʈ�� Building �±��� ��쿡�� ������ ����
	if (object->GetType() != GameObjectInfo::Type::Building)
		return;

	// BFS�� ���� �ֺ� Ÿ�ϵ� ������Ʈ
	std::queue<Path::Pos> q;
	std::map<Path::Pos, bool> visited;

	// ������Ʈ�� �浹 �ڽ�
	const auto& Collider = object->GetCollider();
	auto& Box = Collider->GetBoundingBoxList();
	
	for (int i = 0; i < Box.size(); ++i) {
		// ������Ʈ�� Ÿ�� ���� �ε��� ���
		Vec3		pos = Box[i].Center;
		Path::Pos	index = GetTileUniqueIndexFromPos(pos);
		SetTileFromUniqueIndex(index, tile);
		q.push(index);

		// q�� �� ������ BFS�� ���� ���� Ÿ���� ������Ʈ�� �浹 �ߴٸ� �ش� Ÿ���� ������Ʈ
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

				BoundingBox b2{ nextPosW, Vec3{mkTileWidth, mkTileWidth, mkTileHeight} };

				if (Box[i].Intersects(b2)) {
					SetTileFromUniqueIndex(nextPosT, tile);
					q.push(nextPosT);
				}
			}
		}

	};



}
