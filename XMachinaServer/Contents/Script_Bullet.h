#pragma once
#include "Script.h"
#include "GamePlayer.h"

#include "DB_Weapon.h"

namespace BulletInfo {

}

class Script_Bullet : public Script
{
private:
	SPtr<GamePlayer>	mPlayerOwner{};	// �Ѿ��� �߻��� ��ü (�ڽ��� �浹���� �ʵ��� �Ѵ�)
	SPtr<Rigidbody>		mRigid;
	std::array<std::vector<std::string>, WeaponInfo::BulletPSTypeCount> mPSNames;

	float mSpeed{};					// speed of bullet
	float mDamage{};				// damage of bullet
	float mMaxLifeTime{ 2.f };		// �Ѿ� ����(�߻� �� �ִ� �����ð�)
	float mCurrLifeTime{};			// ���� ����


public:
	Script_Bullet();
	Script_Bullet(SPtr<GameObject> owner, ScriptInfo::Type type);
	~Script_Bullet();

public:
	/// +------------------------------
	///		  virtual function 
	/// ------------------------------+
	virtual void Clone(SPtr<Component> other);

	virtual void Activate();
	virtual void DeActivate();

	virtual bool WakeUp();
	virtual bool Start();
	virtual bool Update();
	virtual void OnDestroy();


	/// +------------------------------
	///		  Bullet Script Funcs  
	/// ------------------------------+
public:
	float GetDamage() const { return mDamage; }

	void SetDamage(float damage) { mDamage = damage; }
	void SetSpeed(float speed) { mSpeed = speed; }
	void SetOwner(SPtr<GamePlayer> object) { mPlayerOwner = object; }
	void SetLifeTime(float lifeTIme) { mMaxLifeTime = lifeTIme; }

public:
	virtual void Init();

	// [pos] ��ġ�� �����ϰ� [dir, up]�� ���� look ������ �����ϰ�, look �������� [speed]�� �ӵ��� �̵��ϵ��� �Ѵ�.
	void Fire(const Vec3& pos, const Vec3& dir, const Vec3& up);
	// [err] ��ŭ ������ ź�� ������.
	void Fire(const Transform& transform, const Vec2& err = Vector2::Zero);

	// �Ѿ� ��ü�� �Ͷ߸���. (���� ó��)
	virtual void Explode();

protected:
	virtual void StartFire() {};

private:
	bool IsPlayerOwner(int playerID) { return playerID == mPlayerOwner->GetID(); }

	void Reset();
	// terrain�� �浹 ���θ� �˻��Ѵ�.
	bool IntersectTerrain();

};

