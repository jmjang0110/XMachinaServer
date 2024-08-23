#include "pch.h"
#include "Script_Bullet.h"
#include "ResourceManager.h"
#include "TimeManager.h"
#include "ServerLib/ThreadManager.h"
#include "GameObject.h"


Script_Bullet::Script_Bullet()
{
    mKey = uint32_t_ScriptKey(ScriptKey::Bullet);
}

Script_Bullet::Script_Bullet(SPtr<GameObject> owner)
    : Script_Entity(owner, uint32_t_ScriptKey(ScriptKey::Bullet))
{
    owner->EnableTag(ObjectTag::Bullet);

}

Script_Bullet::~Script_Bullet()
{
}

SPtr<Component> Script_Bullet::Clone(SPtr<Component> target)
{
    // Try to cast the target to Script_Bullet
    auto clonedScript = std::dynamic_pointer_cast<Script_Bullet>(target);
    if (clonedScript)
    {
        // Call the base class Clone method first
        Script_Entity::Clone(clonedScript);

        // Copy the specific member variables
        clonedScript->mSpeed        = this->mSpeed;
        clonedScript->mDamage       = this->mDamage;
        clonedScript->mMaxLifeTime  = this->mMaxLifeTime;
        clonedScript->mCurrLifeTime = this->mCurrLifeTime;

        // If `mRigid` needs to be cloned, you should also handle it here.
        // For simplicity, assuming `mRigid` does not require special handling.

        return clonedScript;
    }
    else
    {
        std::cout << "Clone failed: target is not of type Script_Bullet" << std::endl;
        return nullptr;
    }
}

void Script_Bullet::Clone(SPtr<GameObject> target)
{
    auto clonedScript = target->AddScript<Script_Bullet>();
    this->Clone(clonedScript);
    clonedScript->SetOwner(target);
}

void Script_Bullet::Update()
{
    Script::Update();

	mCurrLifeTime += DeltaTime();

	if (mCurrLifeTime >= mMaxLifeTime) {
		Reset();
	}
	else if ((OwnerTransform()->GetPosition().y < 0.f) /* || IntersectTerrain()*/) {
		Explode();
	}

}

void Script_Bullet::Init()
{
    mRigid = mOwner->GetRigidbody();
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
