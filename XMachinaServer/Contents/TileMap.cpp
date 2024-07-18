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

}
