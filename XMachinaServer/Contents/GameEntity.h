#pragma once

/// +-------------------------------
///		     GameEntity 
/// ________________________________
///	> [ 설명 ] 
/// - NetworkObject 를 상속받는다.
/// - NetworkObject 를 상속받아야 PQCS 로 Overlapped를 등록할 때 
/// - GQCS 에서 Dispatch를 할 수 있다. 
/// - 왜냐하면 SPtr_NetworkObject 로 Owner 를 받아서 Dispatch 때문이다.
/// _________________________________
///	> [ 구조 ] 
///				   NetworkObject (Dispatch를 통해서 WorkerThread에서 수행가능)
///						↓
///					GameEntity
///						↓
///			 		GameObject 
///				↓			    ↓ 
///			GameMonster		GamePlayer 
///				↓
///		Monster_Ursacetus
/// -------------------------------+

// Overlapped 에 넣을 때 Owner로 저장하기 위해서 .. ( Socket 관련 네트워크 기능은 사용하지 않음 )
class GameEntity : public NetworkObject 
{
private:

	int32_t						mID         = -1;
	std::string					mName       = {};
	bool						mIsActive	= false;

public:
	GameEntity();
	GameEntity(const GameEntity& other);
	GameEntity(int id);

	virtual ~GameEntity();

public:
	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;


public:

	void SetID(int id) { mID = id; }
	void SetName(std::string name) { mName = name; }

	int GetID() { return  mID; }
	std::string GetName() { return mName; }

public:
	bool IsActive() { return mIsActive; }
	virtual void Activate();
	virtual void DeActivate();

};

