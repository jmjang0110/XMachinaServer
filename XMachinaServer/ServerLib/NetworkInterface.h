#pragma once

/// +-----------------------------------------------
///					NetworkInterface 
/// ________________________________________________

/// -----------------------------------------------+


class NetworkInterface : public std::enable_shared_from_this<NetworkInterface>
{
	enum class EnumInfo 
	{
		Max_Session_Limit_Count 
	};

private:
	HANDLE		mIocpHandle    = {};
	SOCKADDR_IN mSockAddr      = {};

	UINT32		mCurSessionCnt = 0;
	UINT32		mMaxSessionCnt = 0;

	std::unordered_map<std::wstring, SPtr_Session> mSessions    = {}; // Key : Ip / Value : Session Obj
	std::function<SPtr_Session(void)>	mSessionConstructorFunc = {}; // Session ������ �Լ� ������ ( session���Ŭ���� ������ )

public:
	NetworkInterface() = default;
	NetworkInterface(std::wstring ip, UINT16 portNum);
	virtual ~NetworkInterface();

public:
	virtual bool Start() abstract;
	virtual void Close();

	void BroadCast(SPtr_SendPktBuf sendBuf);
	void Send(UINT32 sessionID, SPtr_SendPktBuf sendBuf);

	bool RegisterIocp(SPtr_NetObj netObj);
	SPtr_Session CreateSession();
	void AddSession(SPtr_Session session);
	void ReleaseSession(SPtr_Session session);

	/// +---------------------------
	///			   S E T 
	/// ---------------------------+
	void SetMaxSessionCnt(UINT32 sessionLimit) { mMaxSessionCnt = sessionLimit; }
	void SetSessionConstructorFunc(std::function<SPtr_Session(void)> func) { mSessionConstructorFunc = func; }


	/// +---------------------------
	///			   G E T 
	/// ---------------------------+
	UINT32 GetCurSessionCnt() { return mCurSessionCnt; }
	UINT32 GetMaxSEssionCnt() { return mMaxSessionCnt; }
	HANDLE GetIocpHandle()	  { return mIocpHandle; }
	SOCKADDR_IN GetSockAddr() { return mSockAddr; }

};

