#include "pch.h"
#include "Script_Bullet.h"
#include "ResourceManager.h"
#include "TimeManager.h"
#include "ServerLib/ThreadManager.h"
#include "Transform.h"
#include "Component.h"


Script_Bullet::Script_Bullet()
    : Script()
{
}

Script_Bullet::Script_Bullet(SPtr<GameObject> owner, ScriptInfo::Type type)
    : Script(owner, type, static_cast<int>(ScriptInfo::Type::Bullet))
{
}

Script_Bullet::~Script_Bullet()
{
}

void Script_Bullet::Clone(SPtr<Component> other)
{
    Script::Clone(other);

}

void Script_Bullet::Activate()
{
    Script::Activate();

}

void Script_Bullet::DeActivate()
{
    Script::DeActivate();

}

bool Script_Bullet::WakeUp()
{
    Script::WakeUp();

    return false;
}

bool Script_Bullet::Start()
{
    Script::Start();

    return false;
}

bool Script_Bullet::Update()
{
    Script::Update();

	mCurrLifeTime += GetOwner()->GetDeltaTime();

	if (mCurrLifeTime >= mMaxLifeTime) {
		Reset();
	}
	else if ((GetOwner()->GetTransform()->GetPosition().y < 0.f) /* || IntersectTerrain()*/) {
		Explode();
	}

    return true;

}

void Script_Bullet::OnDestroy()
{
    Script::OnDestroy();

}

void Script_Bullet::Init()
{

    mRigid = GetOwner()->GetComponent<Rigidbody>(ComponentInfo::Type::Rigidbody);
    if (mRigid == nullptr)
        assert(0);

    mRigid->SetFriction(0.001f);
    mRigid->SetDrag(0.001f);

    Reset();

}

void Script_Bullet::Fire(const Vec3& pos, const Vec3& dir, const Vec3& up)
{
    GetOwner()->GetTransform()->SetPosition(pos);

    mRigid->Stop();
    mRigid->AddForce(dir, mSpeed, ForceMode::Impulse);

    SetDamage(mDamage);
    StartFire();

}

void Script_Bullet::Fire(const Transform& transform, const Vec2& err)
{
}

void Script_Bullet::Explode()
{
}

void Script_Bullet::Reset()
{
	mCurrLifeTime = 0.f;

}

bool Script_Bullet::IntersectTerrain()
{

    return true;
}
