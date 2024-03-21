#pragma once

/// +-----------------------------------------------
///				   OverlappedObject 
/// ________________________________________________
/// Iocp �Լ��� ����� Overlapped Object �̴�.
/// 
/// �ش� ������Ʈ�� ::GetQueuedCompletionStatus�� ���ؼ�
/// ���� �� IoType�� ���ؼ� � Ÿ������ �� �� �ִ�. 
/// �ش� IoType �� ���� Overlapped_(Accpet, Connect,,)�� 
/// ����ȯ�Ͽ� �ʿ��� �����͸� �����
/// ________________________________________________
/// ���� 
///					OVERLAPPED 
///						��
///				 OverlappedObject
///						��
///				 Overlapped_Accept
///				 Overlapped_Connect
///				 Overlapped_DisConnect
///				 Overlapped_Send
///				 Overlapped_Recv
/// 
/// ----------------------------------------------+

namespace OverlappedIO {
	enum class Type : UINT8 {
		Accept,
		Connect,
		DisConnect,
		Send,
		Recv,
	};
};


class OverlappedObject : public OVERLAPPED
{
private:
	OverlappedIO::Type  mIoType;
	SPtr_NetObj			mOwner;		// Session? Listener?

public:
	OverlappedObject(OverlappedIO::Type ioType);
	virtual ~OverlappedObject();

public:
	OverlappedIO::Type& GetIoType() { return mIoType; }
	void Clear_OVERLAPPED();
	void SetOwner(SPtr_NetObj owner) { mOwner = owner; }

};

/* Accept Overlapped Obeject */
class Overlapped_Accept : public OverlappedObject 
{
public:
	Overlapped_Accept() : OverlappedObject(OverlappedIO::Type::Accept)
	{
	}
	virtual ~Overlapped_Accept()
	{
	}
};

/* Connect Overlapped Object */
class Overlapped_Connect : public OverlappedObject
{
public:
	Overlapped_Connect() : OverlappedObject(OverlappedIO::Type::Connect)
	{
	}
	virtual ~Overlapped_Connect()
	{
	}
};

/* DisConnect Overlapped Object */
class Overlapped_DisConnect : public OverlappedObject
{
public:
	Overlapped_DisConnect() : OverlappedObject(OverlappedIO::Type::DisConnect)
	{
	}
	virtual ~Overlapped_DisConnect()
	{
	}
};

/* Send Overlapped Object */
class Overlapped_Send : public OverlappedObject
{
public:
	Overlapped_Send() : OverlappedObject(OverlappedIO::Type::Send)
	{
	}
	virtual ~Overlapped_Send()
	{
	}
};

/* Recv Overlapped Object */
class Overlapped_Recv : public OverlappedObject
{
public:
	Overlapped_Recv() : OverlappedObject(OverlappedIO::Type::Recv)
	{
	}
	virtual ~Overlapped_Recv()
	{
	}
};