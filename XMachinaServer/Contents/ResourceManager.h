#pragma once
#undef max
#include "Enum_generated.h"

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
class AnimationClip;
class AnimatorController;
class GameItem;

enum class ObjectTag { None, Building, Enemy, Item };

struct ScriptParameter {
	enum class Type : BYTE {
		None,
		String,
		Float,
		Int,
		Bool,
	};

	Type Type{};
	std::string Str{};
	union value {
		bool b;
		int i;
		float f;
	} Val{};
};

class ScriptExporter {
private:
	std::string mName{};
	std::unordered_map<std::string, ScriptParameter> mData{};

public:
	const std::string& GetName() { return mName; }
	template<class T>
	void GetData(const std::string& key, T& out)
	{
		if (!mData.contains(key)) {
			std::cout << "[ERROR - ScriptExporter] data name mismatch.\n";
			assert(0);
			return;
		}
		auto& data = mData[key];

		if constexpr (std::is_same_v<T, std::string>) {
			out = data.Str;
		}
		else if constexpr (std::is_same_v<T, int>) {
			out = data.Val.i;
		}
		else if constexpr (std::is_same_v<T, float>) {
			out = data.Val.f;
		}
		else if constexpr (std::is_same_v<T, bool>) {
			out = data.Val.b;
		}
	}

public:
	void Load(std::ifstream& file);
};

	
class Model {
public:
	Matrix			 mTransform{};	// local ��ġ
	std::string		 mAnimatorController{""};
	std::string		 mName{};
	MyBoundingSphere mBS{};
	std::vector<MyBoundingOrientedBox>	mBoxList{};

public:

	Vec3 GetPosition()  { return Vec3(mTransform._41, mTransform._42, mTransform._43); }
	Vec3 GetRotation()  { return Quaternion::ToEuler(Quat::CreateFromRotationMatrix(mTransform)); }

	Vec3 GetRight()		{ return Vector3::Normalized(Vec3(mTransform._11, mTransform._12, mTransform._13)); }
	Vec3 GetUp()		{ return Vector3::Normalized(Vec3(mTransform._21, mTransform._22, mTransform._23)); }
	Vec3 GetLook()		{ return Vector3::Normalized(Vec3(mTransform._31, mTransform._32, mTransform._33)); }
};

class BattleScene {
private:
	std::vector<SPtr<GameObject>>	mBuildings{};
	std::vector<SPtr<GameMonster>>	mEnemies{};
	std::vector<SPtr<GameItem>>		mStaticItems{};
	std::vector<SPtr<GameItem>>		mDynamicItems{};


public:
	void Load();

	// Getter for mBuildings
	const std::vector<SPtr<GameObject>>* GetBuildings() const {
		return &mBuildings;
	}

	// Getter for mEnemies
	const std::vector<SPtr<GameMonster>>* GetEnemies() const {
		return &mEnemies;
	}

	// Getter for mStaticItems
	const std::vector<SPtr<GameItem>>* GetStaticItems() const {
		return &mStaticItems;
	}

	// Getter for mDynamicItems
	const std::vector<SPtr<GameItem>>* GetDynamicItems() const {
		return &mDynamicItems;
	}

private:
	void AddMonster(SPtr<GameObject> object);
	void AddBuilding(SPtr<GameObject> object);
	void UpdateTiles() const;

	void LoadScriptExporter(std::ifstream& file, SPtr<GameObject> object);
	FBProtocol::ITEM_TYPE GetItemType(std::string itemname);

};


class ResourceManager
{
	DECLARE_SINGLETON(ResourceManager);


private:
	SPtr<HeightMapImage>										mHeightMapImg;
	SPtr<TileMap>												mTileMap;
	std::unordered_map<std::string, SPtr<Model>>				mModels{};
	std::unordered_map<std::string, SPtr<AnimationClip>>		mAnimationClips{};
	std::unordered_map<std::string, SPtr<AnimatorController>>	mAnimatorControllers{};
	SPtr<BattleScene>											mBattleScene{};

public:
	ResourceManager();
	~ResourceManager();

public:
	void Init();

public:
	/// +--------------------------------------------------
	///	�� Getter
	/// --------------------------------------------------+
	SPtr<HeightMapImage>		GetHeightMapImage() const								{ return mHeightMapImg; }
	SPtr<TileMap>				GetTileMap() const										{ return mTileMap; }
	SPtr<Model>					GetModel(const std::string& modelName) const			{ return mModels.at(modelName); }
	SPtr<BattleScene>			GetBattleScene() const 									{ return mBattleScene; }
	SPtr<AnimationClip>			GetAnimationClip(const std::string& name) const			{ return mAnimationClips.at(name); }
	SPtr<AnimatorController>	GetAnimatorController(const std::string& name) const	{ return mAnimatorControllers.at(name); }

private:
	void LoadTerrain();
	void LoadModels();
	void LoadAnimationClips();
	void LoadAnimatorControllers();
};

