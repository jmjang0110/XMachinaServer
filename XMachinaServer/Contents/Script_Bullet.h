#pragma once
#include "Script_Entity.h"

class Rigidbody;
class Script_Bullet : public Script_Entity
{
private:
	SPtr<Rigidbody>		mRigid = nullptr;

	float mSpeed{};					// speed of bullet
	float mDamage{};				// damage of bullet
	float mMaxLifeTime{ 2.f };		// �Ѿ� ����(�߻� �� �ִ� �����ð�)
	float mCurrLifeTime{};			// ���� ����


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

	void SetDamage(float damage)		{ mDamage      = damage; }
	void SetSpeed(float speed)			{ mSpeed       = speed; }
	void SetLifeTime(float lifeTIme)	{ mMaxLifeTime = lifeTIme; }

public:
	virtual void Init();
	void Fire(const Vec3& pos, const Vec3& dir, const Vec3& up); // [pos] ��ġ�� �����ϰ� [dir, up]�� ���� look ������ �����ϰ�, look �������� [speed]�� �ӵ��� �̵��ϵ��� �Ѵ�.
	void Fire(const Transform& transform, const Vec2& err = Vector2::Zero); // [err] ��ŭ ������ ź�� ������.
	virtual void Explode(); // �Ѿ� ��ü�� �Ͷ߸���. (���� ó��)

protected:

	virtual void StartFire() {};
private:
	void Reset();
	// terrain�� �浹 ���θ� �˻��Ѵ�.
	bool IntersectTerrain();

};

