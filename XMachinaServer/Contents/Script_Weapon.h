#pragma once
#include "Script_Item.h"


class GameObject;
namespace WeaponInfo {
	constexpr int MaxBulletsNum = 30;
}

class Script_Weapon : public Script_Item
{
protected:
	int															mMaxBullet   = {};
	std::array<SPtr<GameObject>, WeaponInfo::MaxBulletsNum>		mBullets     = {};			// Bullets 
	
	Concurrency::concurrent_queue<int>							mPossibleBulletIndex;		// Possible To Shoot Bullets 

	float														mAttackRate         = {};
	float														mMaxReloadTime      = {};
	float														mMaxDistance        = {};
	int															mMaxMag             = {};
	int															mBulletCountPerMag  = {};
	int															mBulletCountPerShot = {};

public:
	Script_Weapon() = default;
	Script_Weapon(SPtr<GameObject> owner);
	virtual ~Script_Weapon();

public:
	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);

	virtual void Start();
	virtual void Update();

public:
	virtual int OnHitEnemy(int32_t checktargetID, Vec3& center_pos, Vec3& fire_dir);
	virtual int	OnShoot(Vec3& center_pos, Vec3& fire_dir);
	virtual bool DoInteract(SPtr<GameObject> player) override;


public:
	void SetDataFromDataBase(std::wstring weapon_name);
	void ReturnPossibleBulletIndex(int index);
	
	void SetOwnerPlayer(SPtr<GameObject> owner) { mOwnerPlayer = owner; }
	SPtr<GameObject> GetOwnerPlayer() { return mOwnerPlayer; }
};

