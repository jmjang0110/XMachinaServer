#pragma once
#include "Script.h"

/// +-------------------------------
///		     Script_Entity 
/// ________________________________
///	> [ 설명 ] 
///		Main Script 
///		Dispatch를 통해 실행되는 GameObject
///		의 Main 실행 역할을 담당한다.
/// -------------------------------+

class GameObject;
class Script_Entity : public Script
{
protected:
	EntityTag			mTag			= EntityTag::Untagged;
	Coordinate			mSectorIndex    = {};	
	std::atomic_int		mActivateRef    = 0;

public:
	Script_Entity() = default;
	Script_Entity(SPtr<GameObject> owner, UINT32 key);
	virtual ~Script_Entity();


	virtual SPtr<Component> Clone(SPtr<Component> target);

public:
	virtual void	Activate();
	virtual void	DeActivate();

public:

	/// ---------------- SET ----------------
	void SetTag(EntityTag tag)		{ mTag  = tag; }
	void EnableTag(EntityTag tag)	{ mTag |= tag; }


	/// ---------------- GET ----------------
	EntityTag	GetTag() const						{ return mTag; }
	bool		IsTagEnabled(EntityTag tag) const	{ return (mTag & tag) != 0; }
	Coordinate	GetSectorIndex() const				{ return mSectorIndex; }
	int			GetActivateRef() const				{ return mActivateRef.load(); }
};

