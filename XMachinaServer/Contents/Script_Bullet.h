#pragma once
#include "Script_Entity.h"

class Rigidbody;
enum class BulletType {
	None,
	Basic,
	RayCheck,
	Explosive,
	_count,
};

class Script_Bullet : public Script_Entity
{
protected:
	SPtr<GameObject>	mOwnerWeapon   = nullptr;
	BulletType			mBulletType    = BulletType::None;
	bool				mIsExploded    = false;
protected:
	SPtr<Rigidbody>		mRigid         = nullptr;

	float			    mSpeed         = {};			// speed of bullet
	float			    mDamage        = {};			// damage of bullet
	float			    mMaxLifeTime   = 2.f;		// 총알 수명(발사 후 최대 유지시간)
	float			    mCurrLifeTime  = {};			// 현재 수명


public:
	Script_Bullet();
	Script_Bullet(SPtr<GameObject> owner);
	virtual ~Script_Bullet();

public:
	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);

	virtual void Start() override;
	virtual void Update() override;

public:
	float GetDamage() const { return mDamage; }

	void SetDamage(float damage)				 { mDamage      = damage; }
	void SetSpeed(float speed)					 { mSpeed       = speed; }
	void SetLifeTime(float lifeTIme)			 { mMaxLifeTime = lifeTIme; }
	void SetOwnerWeapon(SPtr<GameObject> weapon) { mOwnerWeapon = weapon; }
public:
	virtual void Init();
	void Fire(const Vec3& pos, const Vec3& dir, const Vec3& up); // [pos] 위치에 생성하고 [dir, up]에 따라 look 방향을 결정하고, look 방향으로 [speed]의 속도로 이동하도록 한다.
	void Fire(const Transform& transform, const Vec2& err = Vector2::Zero); // [err] 만큼 각도로 탄이 퍼진다.
	virtual void Explode(); // 총알 객체를 터뜨린다. (폭발 처리)
	
	bool IsExploded() { return mIsExploded; }

protected:

	virtual void StartFire() {};
private:
	void Reset();
	void ReturnPossibleIndexToWeapon();

	// terrain과 충돌 여부를 검사한다.
	bool IntersectTerrain();
	bool CollideCheck(ObjectTag objTag);

};

