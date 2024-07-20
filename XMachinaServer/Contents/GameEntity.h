#pragma once

/// +-------------------------------
///		     GameEntity 
/// ________________________________
///	> [ ���� ] 
/// - NetworkObject �� ��ӹ޴´�.
/// - NetworkObject �� ��ӹ޾ƾ� PQCS �� Overlapped�� ����� �� 
/// - GQCS ���� Dispatch�� �� �� �ִ�. 
/// - �ֳ��ϸ� SPtr_NetworkObject �� Owner �� �޾Ƽ� Dispatch �����̴�.
/// _________________________________
///	> [ ���� ] 
///				   NetworkObject (Dispatch�� ���ؼ� WorkerThread���� ���డ��)
///						��
///					GameEntity
///						��
///			 		GameObject 
///				��			    �� 
///			GameMonster		GamePlayer 
///				��
///		Monster_Ursacetus
/// -------------------------------+

// Overlapped �� ���� �� Owner�� �����ϱ� ���ؼ� .. ( Socket ���� ��Ʈ��ũ ����� ������� ���� )
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

