#pragma once
#undef max	
#include "Enum_generated.h"

#include "GameObject.h"

class NPCController;
class GameItem : public GameObject
{
public:
	enum class State : UINT8 {
		None, Dropped, PlayerOwned, InCrate, _count,
	};

private:
	NPCController*			mOwnerNC{};

private:
	FBProtocol::ITEM_TYPE	mItemType	= FBProtocol::ITEM_TYPE_NONE;
	GameItem::State			mItemState  = GameItem::State::None;
	
	int	mParentID = -1;
	int mChildID  = -1;

public:
	GameItem();
	GameItem(UINT32 id, Coordinate sectorIdx); /* Monster 생성 아이디 - (생성되고 소멸될 때 까지 임시 아이디)*/
	virtual ~GameItem();

public:
	virtual SPtr<GameItem> Clone();

	void DoInteract(SPtr<GamePlayer> gamePlayer);


public:
	void SetItemType(FBProtocol::ITEM_TYPE itemType)	{ mItemType		= itemType; }
	void SetOwnerNPCController(NPCController* nc)		{ mOwnerNC		= nc;		}
	void SetChildItemID(int id)							{ mChildID      = id; }
	void SetParentItemID(int id)						{ mParentID     = id; }
	void SetItemState(GameItem::State state)			{ mItemState    = state; }

public:
	FBProtocol::ITEM_TYPE GetItemType()					{ return mItemType; }


};
