#pragma once
#include "Script_Item.h"

enum class CrateState : UINT8{
	None,
	Closed,
	Open,
	_count,
};

class GameObejct;
class Script_Crate : public Script_Item
{
private:
	CrateState			mCrateState = CrateState::None;
	SPtr<GameObject>	mItem       = nullptr; Lock::SRWLock mLock_Item;
	int32_t				mItemID     = -1;
public:
	Script_Crate() = default;
	Script_Crate(SPtr<GameObject> owner);
	virtual ~Script_Crate();

public:
	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);
	
	virtual void Start() override;


	virtual bool DoInteract(SPtr<GameObject> player) override;
	bool DropItem(SPtr<GameObject> player);

public:
	/// ---------------------- Set ----------------------
	void		SetItem(SPtr<GameObject> item)  { mLock_Item.LockWrite();  mItem = item; mLock_Item.UnlockWrite(); }
	void		OpenCrate()						{ mCrateState = CrateState::Open; }
	void		CloseCrate()					{ mCrateState = CrateState::Closed; }

	/// ---------------------- Get ----------------------
	SPtr<GameObject> GetItem()			{ mLock_Item.LockRead(); SPtr<GameObject> item = mItem; mLock_Item.UnlockRead(); return item; }
	CrateState		 GetCrateState()	{ return mCrateState; }

};

