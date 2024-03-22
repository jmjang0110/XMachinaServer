#pragma once

/// +-----------------------------------------------
///				   OverlappedObject 
/// ________________________________________________
/// Iocp 함수에 등록할 Overlapped Object 이다.
/// 
/// 해당 오브젝트를 ::GetQueuedCompletionStatus를 통해서
/// 받을 때 IoType을 통해서 어떤 타입인지 알 수 있다. 
/// 해당 IoType 을 통해 Overlapped_(Accpet, Connect,,)로 
/// 형변환하여 필요한 데이터를 사용함
/// ________________________________________________
/// 구조 
///					OVERLAPPED 
///						↓
///				 OverlappedObject
///						↓
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
	SPtr_NetObj GetOwner() { return mOwner; }


};

/* Accept Overlapped Obeject */
class Overlapped_Accept : public OverlappedObject 
{
private:
	SPtr_Session  mSession;

public:
	SPtr_Session GetSession() { return mSession; }
	void SetSession(SPtr_Session session) { mSession = session; }

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