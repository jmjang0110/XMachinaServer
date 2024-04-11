#pragma once
class GameRoom
{
	enum class RoomInfo : UINT8 {
		RoomSize = 10
	};

private:
	concurrency::concurrent_unordered_map<int, SPtr_GameObject> mGameObjects{};


public:
	void InsertGameObject(int id, SPtr_GameObject obj);
	SPtr_GameObject FindGameObject(int id);
	
	void Init();
	void Update();

};

