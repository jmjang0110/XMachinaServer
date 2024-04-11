#include "pch.h"
#include "GameRoom.h"

void GameRoom::InsertGameObject(int id, SPtr_GameObject obj)
{
	mGameObjects.insert({ id, obj });

}

SPtr_GameObject GameRoom::FindGameObject(int id)
{
	int size = mGameObjects.size();
	auto obj = mGameObjects.find(id);
	if (obj == mGameObjects.end())
		return nullptr;
	return obj->second;
}

void GameRoom::Init()
{
}

void GameRoom::Update()
{
}
