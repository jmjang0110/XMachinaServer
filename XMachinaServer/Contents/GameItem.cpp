#include "pch.h"
#include "GameItem.h"
#include "FBsPacketFactory.h"
#include "GameManager.h"
#include "NPCController.h"

GameItem::GameItem()
	: GameObject(-1)
{
}

GameItem::GameItem(UINT32 id, Coordinate sectorIdx)
	: GameObject(id)
{
}

GameItem::~GameItem()
{
}

SPtr<GameItem> GameItem::Clone()
{

	SPtr<GameItem> copy = std::make_shared<GameItem>();

	copy->SetID(GetID());
	copy->SetType(this->GetType());

	copy->mItemState           = this->mItemState;
	copy->mItemType            = this->mItemType;
	copy->mChildID             = this->mChildID;
	copy->mParentID            = this->mParentID;

	SPtr<GameObject> targetObj = std::dynamic_pointer_cast<GameObject>(copy);
	GameObject::CloneComponents(targetObj);
	GameObject::CloneScripts(targetObj);

	return copy;
}

void GameItem::DoInteract(SPtr<GamePlayer> gamePlayer)
{
	switch (mItemType)
	{
	case FBProtocol::ITEM_TYPE_NONE:
		break;
	case FBProtocol::ITEM_TYPE_STATIC_ITEM:
		break;
	case FBProtocol::ITEM_TYPE_DYNAMIC_ITEM:
		break;
	case FBProtocol::ITEM_TYPE_STATIC_ITEM_CRATE: {
		if (mItemState == GameItem::State::InCrate) {
			mItemState = GameItem::State::Dropped;
			// 상자가 열림 
			auto spkt = FBS_FACTORY->SPkt_Item_Interact(gamePlayer->GetID(),GetID(), mItemType,GetTransform()->GetPosition());
			GAME_MGR->BroadcastRoom(gamePlayer->GetRoomID(), spkt);

			// 아이템이 드랍됐음을 알린다.
			auto weapon = mOwnerNC->GetDynamicItem(mChildID);
			weapon->SetItemState(GameItem::State::Dropped);
			spkt = FBS_FACTORY->SPkt_Item_ThrowAway(0, weapon->GetID(), weapon->GetItemType(), weapon->GetTransform()->GetPosition());
			GAME_MGR->BroadcastRoom(gamePlayer->GetRoomID(), spkt);

		}
	}
		break;
	case FBProtocol::ITEM_TYPE_WEAPON_H_LOOK:
	case FBProtocol::ITEM_TYPE_WEAPON_DBMS:
	case FBProtocol::ITEM_TYPE_WEAPON_STUART:
	case FBProtocol::ITEM_TYPE_WEAPON_DESCRIPTOR:
	case FBProtocol::ITEM_TYPE_WEAPON_T_12:
	case FBProtocol::ITEM_TYPE_WEAPON_PIPELINE:
	case FBProtocol::ITEM_TYPE_WEAPON_BURNOUT:
	case FBProtocol::ITEM_TYPE_WEAPON_DIRECT_DRAIN:
	case FBProtocol::ITEM_TYPE_WEAPON_SKYLINE:
	case FBProtocol::ITEM_TYPE_WEAPON_MINE_LAUNCHER:
	case FBProtocol::ITEM_TYPE_WEAPON_AIR_STRIKE:
	{
		if (mItemState == GameItem::State::Dropped) {
			mItemState = GameItem::State::PlayerOwned;
			gamePlayer->S_SetEquipWeapon(mItemType);

			auto spkt = FBS_FACTORY->SPkt_Item_Interact(gamePlayer->GetID(), GetID(), mItemType, GetTransform()->GetPosition());
			GAME_MGR->BroadcastRoom(gamePlayer->GetRoomID(), spkt);
		}
		else if (mItemState == GameItem::State::PlayerOwned) {
			mItemState = GameItem::State::Dropped;
			gamePlayer->S_SetEquipWeapon(FBProtocol::ITEM_TYPE_NONE);

			auto spkt = FBS_FACTORY->SPkt_Item_ThrowAway(gamePlayer->GetID(), GetID(), mItemType, gamePlayer->GetTransform()->GetSnapShot().GetPosition());
			GAME_MGR->BroadcastRoom(gamePlayer->GetRoomID(), spkt);
		}
		
	}
		break;
	default:
		assert(0);
		break;
	}
}

