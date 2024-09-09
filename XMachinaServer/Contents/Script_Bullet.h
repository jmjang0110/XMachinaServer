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
	float			    mMaxLifeTime   = 2.f;		// �Ѿ� ����(�߻� �� �ִ� �����ð�)
	float			    mCurrLifeTime  = {};			// ���� ����


public:
	Script_Bullet();
	Script_Bullet(SPtr<GameObject> owner);
	virtual ~Script_Bullet();

public:
	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);

	virtual void Update();

public:
	float GetDamage() const { return mDamage; }

	void SetDamage(float damage)				 { mDamage      = damage; }
	void SetSpeed(float speed)					 { mSpeed       = speed; }
	void SetLifeTime(float lifeTIme)			 { mMaxLifeTime = lifeTIme; }
	void SetOwnerWeapon(SPtr<GameObject> weapon) { mOwnerWeapon = weapon; }
public:
	virtual void Init();
	void Fire(const Vec3& pos, const Vec3& dir, const Vec3& up); // [pos] ��ġ�� �����ϰ� [dir, up]�� ���� look ������ �����ϰ�, look �������� [speed]�� �ӵ��� �̵��ϵ��� �Ѵ�.
	void Fire(const Transform& transform, const Vec2& err = Vector2::Zero); // [err] ��ŭ ������ ź�� ������.
	virtual void Explode(); // �Ѿ� ��ü�� �Ͷ߸���. (���� ó��)
	
	bool IsExploded() { return mIsExploded; }

protected:

	virtual void StartFire() {};
private:
	void Reset();
	void ReturnPossibleIndexToWeapon();

	// terrain�� �浹 ���θ� �˻��Ѵ�.
	bool IntersectTerrain();
	bool CollideCheck(ObjectTag objTag);

};

