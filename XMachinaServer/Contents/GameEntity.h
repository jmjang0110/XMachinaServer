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
///			GameObject		GameObject 
///				��
///		Monster_Ursacetus
/// -------------------------------+

// Overlapped �� ���� �� Owner�� �����ϱ� ���ؼ� .. ( Socket ���� ��Ʈ��ũ ����� ������� ���� )
class GameEntity : public NetworkObject 
{
protected:
	bool mIsActive	= false;

public:
	GameEntity();
	GameEntity(const GameEntity& other);
	GameEntity(int id);

	virtual ~GameEntity();

public:
	bool IsActive() { return mIsActive; }

	virtual void Activate();
	virtual void DeActivate();

};

