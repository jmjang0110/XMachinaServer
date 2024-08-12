#include "pch.h"
#include "GameItem.h"

GameItem::GameItem()
	: GameObject(-1)
{
}

GameItem::GameItem(UINT32 id, Coordinate sectorIdx)
	: GameObject(id)
{
	mSectorIndex = sectorIdx;
}

GameItem::~GameItem()
{
}
