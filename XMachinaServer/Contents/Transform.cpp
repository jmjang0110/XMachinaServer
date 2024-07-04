#include "pch.h"
#include "Transform.h"

void Transform::UpdateAxis(bool isComputeWorldTransform)
{
	::memcpy(&mRight, &mLocalTransform._11, sizeof(Vec3));
	::memcpy(&mUp, &mLocalTransform._21, sizeof(Vec3));
	::memcpy(&mLook, &mLocalTransform._31, sizeof(Vec3));
	//::memcpy(&mPosition, &mLocalTransform._41, sizeof(Vec3));
}

void Transform::NormalizeAxis()
{
	mLook = Vector3::Normalized(mLook);
	mRight = Vector3::Normalized(mUp.Cross(mLook));
	mUp = Vector3::Normalized(mLook.Cross(mRight));
	UpdateLocalTransform(false);

}

void Transform::UpdateLocalTransform(bool isComputeWorldTransform)
{
	::memcpy(&mLocalTransform._11, &mRight, sizeof(Vec3));
	::memcpy(&mLocalTransform._21, &mUp, sizeof(Vec3));
	::memcpy(&mLocalTransform._31, &mLook, sizeof(Vec3));
	::memcpy(&mLocalTransform._41, &mPosition, sizeof(Vec3));
}

Transform::Transform()
{
}

Transform::Transform(UINT32 id)
	: Component(id)
{
}

Transform::~Transform()
{
}

bool Transform::WakeUp()
{
	Component::WakeUp();

	return true;
}

bool Transform::Start()
{
	Component::Start();

	return true;
}

bool Transform::Update()
{
	Component::Update();

	return true;
}

Vec3 Transform::GetPosition()
{
	return mPosition;
}

Vec3 Transform::GetDirection(DWORD dir, float distance) const
{
	if (dir == Dir::None) {
		return Vector3::Zero;
	}

	Vec3 result{};

	result += static_cast<bool>(dir & Dir::Front) * mLook;
	result += static_cast<bool>(dir & Dir::Back) * -mLook;
	result += static_cast<bool>(dir & Dir::Right) * mRight;
	result += static_cast<bool>(dir & Dir::Left) * -mRight;
	result += static_cast<bool>(dir & Dir::Up) * mUp;
	result += static_cast<bool>(dir & Dir::Down) * -mUp;

	return Vector3::Normalized(result);
}

Vec4 Transform::GetRotation() const
{
	return DirectX::SimpleMath::Quaternion::CreateFromRotationMatrix(mLocalTransform);

	return Vec4();
}

void Transform::Translate(const Vec3& translation)
{
	if (translation.Length() <= FLT_EPSILON) {
		return;
	}

	mPosition += translation;
}

void Transform::Rotate(float pitch, float yaw, float roll)
{
	Matrix mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(pitch), XMConvertToRadians(yaw), XMConvertToRadians(roll));
	mLocalTransform = mtxRotate * mLocalTransform;

	UpdateAxis();
}
