#include "pch.h"
#include "Rigidbody.h"

#include "ServerLib/ThreadManager.h"
#include "TimeManager.h"
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "Collider.h"
#include "TimeManager.h"

Rigidbody::Rigidbody()
	: Component()

{
}

Rigidbody::Rigidbody(const Rigidbody& other)
	: Component(other)

{
}

Rigidbody::Rigidbody(SPtr<GameObject> owner, Component::Type Type)
	: Component(owner, Type, static_cast<UINT32>(Type))

{
}

Rigidbody::~Rigidbody()
{
}

SPtr<Component> Rigidbody::Clone(SPtr<Component> target)
{
	// target�� nullptr�� ���, ���ο� Rigidbody �ν��Ͻ��� �����մϴ�.
	if (!target)
	{
		target = std::make_shared<Rigidbody>(mOwner, mType);
	}

	// �⺻ Component Ŭ������ Clone �޼��带 ȣ���Ͽ� ���� �Ӽ��� �����մϴ�.
	Component::Clone(target);

	// Rigidbody ������ �Ӽ��� �����մϴ�.
	SPtr<Rigidbody> clonedRigidbody = std::dynamic_pointer_cast<Rigidbody>(target);
	if (clonedRigidbody)
	{
		clonedRigidbody->mUseGravity    = mUseGravity;
		clonedRigidbody->mGravityScale  = mGravityScale;
		clonedRigidbody->mMass          = mMass;
		clonedRigidbody->mDrag          = mDrag;
		clonedRigidbody->mFriction      = mFriction;
		clonedRigidbody->mAcc           = mAcc;
		clonedRigidbody->mMaxSpeed      = mMaxSpeed;
		clonedRigidbody->mVelocity      = mVelocity;
		clonedRigidbody->mSnapShotIndex = mSnapShotIndex.load(); // std::atomic_bool ����
	}

	return target;

}

void Rigidbody::Activate()
{
	Component::Activate();

}

void Rigidbody::DeActivate()
{
	Component::DeActivate();
}


void Rigidbody::Start()
{
}

void Rigidbody::Update()
{
	const float gravity = Math::kGravity * mGravityScale;

	// �������� �����Ѵ�.
	if (mFriction > FLT_EPSILON) {
		float normalForce{ mMass };		// �����׷�
		if (mUseGravity) {
			normalForce *= gravity;		// ���� �׷¿� �߷��� �����Ѵ�.
		}

		const float friction = mFriction * normalForce;		// ���� ��� = ���� ��� * �����׷�

		const float mag = mVelocity.Length();			// ���� �ӵ� ũ��
		if (mag > FLT_EPSILON) {						// ���� �ӵ��� �ִٸ�
			const Vec3 frictionDir = -mVelocity / mag;	// ������ ����(���� �ӵ��� �ݴ����)

			const Vec3 frictionForce = frictionDir * friction;			// ������ = ������ ���� * �������
			const Vec3 dragForce = mVelocity * -mDrag;					// ���׷� = �ӵ� * ���� ��� * -1
			const Vec3 dragAcc = dragForce / mMass;							// ���� ���ӵ� = ���׷� * ����

			const Vec3 frictionAcc = (frictionForce / mMass) + dragAcc;	// ���� ���ӵ� = (������/����) + ���� ���ӵ�

			const Vec3 resultVec = mVelocity + (frictionAcc * DeltaTime());	// ��� = ���� �ӵ� + (���� ���ӵ� * DeltaTime)

			// �� ������ ������ �ٲ�� 0���� �����Ѵ�.
			mVelocity.x = (mVelocity.x * resultVec.x < 0) ? 0.f : resultVec.x;
			mVelocity.y = (mVelocity.y * resultVec.y < 0) ? 0.f : resultVec.y;
			mVelocity.z = (mVelocity.z * resultVec.z < 0) ? 0.f : resultVec.z;

			// �߷��� Ȱ��ȭ�� ��� �߷� ���ӵ��� ����� �ӵ��� �����Ѵ�.
			if (mUseGravity) {
				const Vec3 gravityForce = Vector3::Down * normalForce;			// �߷� = �����׷�(-y)
				const Vec3 gravityAcc = gravityForce * mMass;				// �߷� ���ӵ� = �߷� * ����
				mVelocity += gravityAcc * DeltaTime();						// �ӵ� = �ӵ� + (�߷°��ӵ� * DeltaTime)
			}
		}
	}

	if (mVelocity.Length() > FLT_EPSILON) {
		// mVelocity �ӵ��� DeltaTime��ŭ �̵��Ѵ�.
		mOwner->GetTransform()->Translate(mVelocity * DeltaTime());
	}

}

void Rigidbody::LateUpdate()
{
}

void Rigidbody::End()
{
}

void Rigidbody::SetVelocity(float speed)
{
	mVelocity = Vector3::Resize(mVelocity, speed);

}


void Rigidbody::Stop()
{
	SetVelocity(Vector3::Zero);

}

void Rigidbody::AddForce(const Vec3& force, ForceMode forceMode)
{
	float t{ 1 };
	if (forceMode == ForceMode::Accleration) {		// ���ӵ��� ��� DelteTime�� �����ϵ��� �Ѵ�.
		t = DeltaTime();
	}

	const Vec3 acc = ((force * mAcc) / mMass) * t;	// ���ӵ� = ((force * mAcc) / ����) * t
	mVelocity += acc;								// ���� �ӵ��� ���ӵ��� ���Ѵ�.
	if (mVelocity.Length() > mMaxSpeed) {			// �ִ� �ӵ��� ���� �ʵ��� �Ѵ�.
		SetVelocity(mMaxSpeed);
	}
}

void Rigidbody::AddForce(const Vec3& dir, float speed, ForceMode forceMode)
{
	AddForce(dir * speed, forceMode);		// force = dir * speed
}
