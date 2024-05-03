#pragma once


class GameEntity
{
private:

	int						mID = {};
	std::string				mName = {};

public:
	GameEntity();
	GameEntity(int id);

	virtual ~GameEntity();

public:

	void SetID(int id) { mID = id; }
	void SetName(std::string name) { mName = name; }

	int GetID() { return  mID; }
	std::string GetName() { return mName; }
};

