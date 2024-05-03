#pragma once


#include "Key.h"

namespace TransformInfo {
	enum class Movement : DWORD {
		None = 0x00,
		Stand = 0x01,
		Sit = 0x02,
		Walk = 0x10,
		Run = 0x20,
		Sprint = 0x40
	};

	enum class Rotate : DWORD {
		None = 0x00,
		Stop = 0x01,
		Cw = 0x02,
		CCw = 0x10,
	};

	const float SitWalkSpeed = 1.5f;
	const float StandWalkSpeed = 2.2f;
	const float RunSpeed = 5.f;
	const float SprintSpeed = 8.f;
}

class Transform
{
private:
	TransformInfo::Movement		mCurrMoveType = TransformInfo::Movement::None;
	TransformInfo::Movement		mPrevMovement = TransformInfo::Movement::None;
	float						mMovementSpeed = 0.f;

	TransformInfo::Rotate		mRotateType = TransformInfo::Rotate::Stop;

	/* VEC3 : POS / ROT / SCALE */
	Vec3				mPosition = Vec3(100, 0, 100);//Vector3::Zero();
	Vec3				mRotation = Vector3::Zero;
	Vec3				mScale    = Vector3::Zero;

	/* VEC3 : LOOK / UP / RIGHT */
	Vec3				mRight = Vector3::Right;		// right(x) axis in local space
	Vec3				mUp    = Vector3::Up;			// up(y)    axis in local space
	Vec3				mLook  = Vector3::Forward;		// look(z)  axis in local space

	Matrix mLocalTransform = Matrix();

private:
	// set axis vectors from local transform
	void UpdateAxis(bool isComputeWorldTransform = true);
	void NormalizeAxis();

	// set local transform from axis vectors
	void UpdateLocalTransform(bool isComputeWorldTransform = true);

public:
	void SetPosition(Vec3 pos) { mPosition = pos; }
	void SetRotation(Vec3 Rot) { mRotation = Rot; }
	void SetScale(Vec3 Scale) { mScale = Scale; }

public:
	Vec3 GetPosition();
	Vec4 GetRotation() const;
	Vec3 GetDirection(DWORD dir, float distance = 1.f) const;

	void Translate(const Vec3& translation);
	void Rotate(float pitch = 0.f, float yaw = 0.f, float roll = 0.f);

};

