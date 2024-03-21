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

	std::unordered_map<std::wstring, SPtr_Session> mSessions = {}; // Key : Ip / Value : Session Obj

public:
	NetworkInterface() = default;
	NetworkInterface(std::wstring ip, UINT16 portNum);
	virtual ~NetworkInterface();


public:
	void BroadCast(SPtr_SendPktBuf sendBuf);
	void Send(UINT32 sessionID, SPtr_SendPktBuf sendBuf);

	void CreateSession();
	void AddSession(SPtr_Session session);
	void ReleaseSession(SPtr_Session session);


};

