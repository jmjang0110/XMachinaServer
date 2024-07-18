#pragma once

#include "HeightMapImage.h"
#include "TileMap.h"

/// +-------------------------------
///		   ResourceManager
/// ________________________________
///	[ ���� ] 
/// - �ʿ��� ���ҽ��� �ε��� �� �����ؼ� ��������.
/// ________________________________
/// 
/// 
/// -------------------------------+


#define RESOURCE_MGR ResourceManager::GetInst()

class GameObject;

enum class ObjectTag { None, Building, Enemy };
	
class Model {
public:
	Matrix mTransform{};	// local ��ġ
	std::string mName{};
	std::vector<MyBoundingSphere> mBSList{};
	std::vector<MyBoundingOrientedBox> mBoxList{};

	SPtr<Model> mChild{};
	SPtr<Model> mSibling{};

public:
	void SetChild(SPtr<Model> child);
};

class BattleScene {
private:
	std::vector<SPtr<GameObject>> mBuildings{};
	std::vector<SPtr<GameObject>> mEnemies{};

public:
	void Load();
};


class ResourceManager
{
	DECLARE_SINGLETON(ResourceManager);


private:
	SPtr<HeightMapImage> mHeightMapImg;
	SPtr<TileMap>		 mTileMap;
	std::unordered_map<std::string, SPtr<Model>> mModels{};
	BattleScene mBattleScene{};

public:
	ResourceManager();
	~ResourceManager();

public:
	void Init();

public:
	/// +--------------------------------------------------
	///	�� Getter
	/// --------------------------------------------------+
	SPtr<HeightMapImage> GetHeightMapImage() { return mHeightMapImg; }
	SPtr<TileMap>		 GetTileMap()		 { return mTileMap; }
	SPtr<Model> GetModel(const std::string& modelName) const { return mModels.at(modelName); }

private:
	void LoadTerrain();
	void LoadModels();
};

