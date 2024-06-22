#pragma once
class Entity
{
private:
	UINT64		mID;
	std::string mName;

public:
	Entity();
	Entity(std::string name);
	virtual ~Entity();

public:
	std::string GetName();
	void SetName(std::string name);

	UINT64 GetID();
	void SetName(UINT64 id);

	void setID(int id) { mID = id; }
};

