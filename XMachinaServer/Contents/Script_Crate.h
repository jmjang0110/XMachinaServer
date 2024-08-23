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
	SPtr<GameObject>	mItem       = nullptr;

public:
	Script_Crate() = default;
	Script_Crate(SPtr<GameObject> owner);
	virtual ~Script_Crate();

	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);

public:
	/// ---------------------- Set ----------------------
	void SetItem(SPtr<GameObject> item) { mItem       = item; }
	void OpenCrate()					{ mCrateState = CrateState::Open; }
	void CloseCrate()					{ mCrateState = CrateState::Closed; }

	/// ---------------------- Get ----------------------
	SPtr<GameObject> GetItem() { return mItem; }

};

