#pragma once


// Overlapped 에 넣을 때 Owner로 저장하기 위해서 .. ( Socket 관련 네트워크 기능은 사용하지 않음 )
class GameEntity : public NetworkObject 
{
private:

	int						mID = {};
	std::string				mName = {};

public:
	GameEntity();
	GameEntity(int id);

	virtual ~GameEntity();

public:
	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;


public:

	void SetID(int id) { mID = id; }
	void SetName(std::string name) { mName = name; }

	int GetID() { return  mID; }
	std::string GetName() { return mName; }
};

