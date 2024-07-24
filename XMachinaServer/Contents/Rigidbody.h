#pragma once

#include "Component.h"
#include "ObjectSnapShot.h"

enum class ForceMode {
	Accleration = 0,
	Impulse
};


/// +-----------------------------------------------------
///					COLLIDER SNAPSHOT 
/// -----------------------------------------------------+
struct RigidbodySnapShot : public ObjectSnapShot
{
	bool	mUseGravity		= false;	// controls whether gravity affects this rigidbody.
	float	mGravityScale	= 1.f;

	float	mMass			= 1.f;
	float	mDrag			= 1.f;
	float	mFriction		= 1.f;
	float	mAcc			= 1.f;
	float	mMaxSpeed		= FLT_MAX;
	Vec3	mVelocity{};


};

class Rigidbody : public Component
{
private:
	bool	mUseGravity    = false;	// controls whether gravity affects this rigidbody.
	float	mGravityScale  = 1.f;

	float	mMass		   = 1.f;
	float	mDrag		   = 1.f;
	float	mFriction	   = 1.f;
	float	mAcc		   = 1.f;
	float	mMaxSpeed	   = FLT_MAX;
	Vec3	mVelocity{};

private:
	std::atomic_bool  mSnapShotIndex = 0;
	RigidbodySnapShot mRigidbodySnapShot[2]{};

public:
	Rigidbody();
	Rigidbody(const Rigidbody& other);
	Rigidbody(SPtr<GameObject> owner, ComponentInfo::Type Type);
	~Rigidbody();

public:
	virtual void Clone(SPtr<Component> CopyT) ;


	virtual void Activate();
	virtual void DeActivate();

	virtual void OnEnable();
	virtual void OnDisable();

public:
	virtual bool WakeUp();
	virtual bool Start();
	virtual bool Update();
	virtual bool LateUpdate();

	virtual void OnDestroy();

	/// +-----------------------------------
	///	     RIGID BODY SNAP SHOT FUNCS  
	/// -----------------------------------+


	/// +-----------------------------------
	///	 RIGID BODY Functions 
	/// -----------------------------------+
public:
	const Vec3& GetVelocity() const { return mVelocity; }

	void SetGravity(bool useGravity) { mUseGravity = useGravity; }
	void SetGravityScale(float scale) { mGravityScale = scale; }
	void SetMass(float mass) { mMass = mass; }
	void SetDrag(float drag) { mDrag = drag; }
	void SetAcc(float acc) { mAcc = acc; }
	void SetMaxSpeed(float speed) { mMaxSpeed = speed; }
	void SetFriction(float fFriction) { mFriction = fFriction; }
	void SetVelocity(const Vec3& xmf3Velocity) { mVelocity = xmf3Velocity; }
	void SetVelocity(float speed);


	// set velocity to zero immediately
	void Stop();

	// adds a force to the Rigidbody.
	void AddForce(const Vec3& force, ForceMode forceMode = ForceMode::Accleration);
	void AddForce(const Vec3& dir, float speed, ForceMode forceMode = ForceMode::Accleration);

};

