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
class AnimationClip;
class AnimatorController;

enum class ObjectTag { None, Building, Enemy };
	
class Model {
public:
	Matrix								mTransform{};	// local ��ġ
	std::string							mAnimatorController{""};
	std::string							mName{};
	std::vector<MyBoundingSphere>		mBSList{};
	std::vector<MyBoundingOrientedBox>	mBoxList{};

	SPtr<Model>							mChild{};
	SPtr<Model>							mSibling{};

public:
	void SetChild(SPtr<Model> child);


public:

	Vec3 GetPosition()  { return Vec3(mTransform._41, mTransform._42, mTransform._43); }
	Vec3 GetRotation()  { return Quaternion::ToEuler(Quat::CreateFromRotationMatrix(mTransform)); }

	Vec3 GetRight()		{ return Vector3::Normalized(Vec3(mTransform._11, mTransform._12, mTransform._13)); }
	Vec3 GetUp()		{ return Vector3::Normalized(Vec3(mTransform._21, mTransform._22, mTransform._23)); }
	Vec3 GetLook()		{ return Vector3::Normalized(Vec3(mTransform._31, mTransform._32, mTransform._33)); }
};

class BattleScene {
private:
	std::vector<SPtr<GameObject>> mBuildings{};
	std::vector<SPtr<GameMonster>> mEnemies{};

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

