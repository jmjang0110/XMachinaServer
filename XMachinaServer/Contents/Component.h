#pragma once

class GameObject;
class Component : public std::enable_shared_from_this<Component>
{
public:
	enum class Type : UINT16 {
		None, Transform, Collider, Animation, Rigidbody, Script, _count, 
	};

protected:
	UINT32				mID       = -1;
	std::string			mName     = {};
	Type				mType	  = Type::None;
	SPtr<GameObject>	mOwner	  = nullptr;
	bool				mIsActive = false;

public:
	Component();
	Component(SPtr<GameObject> owner, Type Type, UINT32 id);
	virtual ~Component();

public:
	virtual SPtr<Component>	Clone(SPtr<Component> target);

	virtual void	Activate();
	virtual void	DeActivate();

	virtual void	Start() ;
	virtual void	Update();
	virtual void	LateUpdate();
	virtual void	End();

public:
	float DeltaTime();

public:
	void			SetID(UINT32 id)				 { mID    = id; }
	void			SetOwner(SPtr<GameObject> owner) { mOwner = owner; }
	void			SetType(Type type)				 { mType  = type; }

public:
	UINT32			 GetID()			 { return mID; }
	SPtr<GameObject> GetOwner()			 { return mOwner; }
	bool			 IsActive() const	 { return mIsActive; }

};

