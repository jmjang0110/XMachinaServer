#pragma once

class SectorController;

namespace TileMapInfo
{
	enum class TileType : UINT8 {
		None = 0,
		Static,
	};
}
/// +---------------------------------------------------
///	[][][][][][][][][][][][][][][][][][][][][][][][][][]
/// [][][][][][][][][][][][][][][][][][][][][][][][][][]
/// [][][][][][][][][][][][][][][][][][][][][][][][][][]
/// [][][][][][][][][][][][][][][][][][][][][][][][][][]
/// [][][][][][][][][][][][][][][][][][][][][][][][][][]
/// [][][][][][][][][][][][][][][][][][][][][][][][][][]
/// [][][][][][][][][][][][][][][][][][][][][][][][][][]
/// [][][][][][][][][][][][][][][][][][][][][][][][][][]
/// [][][][][][][][][][][][][][][][][][][][][][][][][][]
/// ----------------------------------------------------+

class GameObject;
class TileMap
{

private:
	int												mIndex   = {};
	std::vector<std::vector<TileMapInfo::TileType>> mTiles   = {};

public:
	float											mTileMapStartPoint = 0;				// leftmost coord of the entire TileMap
	int												mTileMapXLength    = 1500;			// length of x for one TileMap
	int												mTileMapZLength    = 1500;			// length of z for one TileMap

	static constexpr float							mkTileHeight	  = 0.5f;
	static constexpr float							mkTileWidth		  = 0.5f;
	static int										mTileRows;
	static int										mTileCols;

public:
	TileMap();
	virtual ~TileMap() = default;

	int GetIndex() const						{ return mIndex; }

	
	Path::Pos				GetTileUniqueIndexFromPos(const Vec3& pos) const;
	Vec3					GetTilePosFromUniqueIndex(const Path::Pos& index) const;
	TileMapInfo::TileType	GetTileFromUniqueIndex(const Path::Pos& index) const;
	TileMapInfo::TileType   GetTileFromPos(const Vec3& pos) const;

	void					SetTileFromUniqueIndex(const Path::Pos& index, TileMapInfo::TileType  tile);
	

	// BFS�� Ȱ���Ͽ� Ÿ�� ������Ʈ
	void UpdateTiles(TileMapInfo::TileType tile, GameObject* object);


};

