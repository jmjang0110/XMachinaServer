#pragma once
#undef max	
#include "Enum_generated.h"

#include "GameObject.h"

class NPCController;
class GameItem : public GameObject
{
public:
	enum class State : UINT8 {
		Dropped, PlayerOwned, InCrate, _count,
	};

private:
	NPCController*			mOwnerNC;
	Coordinate				mSectorIndex;				// ��� ���Ϳ� �����ִ°�?

private:
	FBProtocol::ITEM_TYPE	mItemType	= FBProtocol::ITEM_TYPE_MIN;
	
	GameItem*				mParent     = nullptr;
	GameItem*				mChild		= nullptr;

public:
	GameItem();
	GameItem(UINT32 id, Coordinate sectorIdx); /* Monster ���� ���̵� - (�����ǰ� �Ҹ�� �� ���� �ӽ� ���̵�)*/
	virtual ~GameItem();

	void SetItemType(FBProtocol::ITEM_TYPE itemType)	{ mItemType		= itemType; }
	void SetOwnerNPCController(NPCController* nc)		{ mOwnerNC		= nc; }
	FBProtocol::ITEM_TYPE GetItemType()					{ return mItemType; }

	void SetChildItem(GameItem* child)					{ mChild		= child; }
};
