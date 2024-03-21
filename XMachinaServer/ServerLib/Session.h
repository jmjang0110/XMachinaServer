#pragma once

/// +-----------------------------------------------
///					   Session 
/// ________________________________________________
///	구조
///					
/// ----------------------------------------------+


#include "SocketData.h"
#include "OverlappedObject.h"
#include "NetworkObject.h"
#include "PacketRecvBuf.h"
#include "PacketSendBuf.h"

class Session : public NetworkObject
{
private:
	/* Check If Client Connect to Server */
	std::atomic<bool>			mIsConnected		= false;

	/* Overlapped I/O Object */
	Overlapped_Connect			mConnect			= {};
	Overlapped_DisConnect		mDisconnect			= {};
	Overlapped_Recv				mRecv				= {};
	Overlapped_Send				mSend				= {};

	/* send Packet Buffer */
	std::queue<SPtr_SendPktBuf> mSendPkt_Queue		= {};
	std::atomic<bool>			mIsSendRegistered	= false;

	/* Recv Packet Buffer */	
	PacketRecvBuf				mRecvPkt			= {};

	/* Server Network or Client Network - Set Owner */
	std::weak_ptr<class NetworkInterface> mOwnerNI	= {};

public:
	Session();
	virtual ~Session();

protected:
	/* Session class을 상속받은 class는 신호를 받는다. */
	virtual void	OnConnected() {};
	virtual void	OnDisconnected() {};
	virtual void	OnSend(UINT32 len) {};
	virtual UINT32	OnRecv(BYTE* buf, UINT32 len) { return len; }


public:
	/* I/O - Register / Process */
	void RegisterIO(OverlappedIO::Type IoType);
	void ProcessIO(OverlappedIO::Type IoType);

	/* Error Code 에 따른 Session 처리 */
	void ProcessError(INT32 errCode);

	/* Network */
	void Send(SPtr_SendPktBuf buf);
	void Connect();
	void Disconnect(const WCHAR* cause);

public:
	/* Set */

	/* Get */
	bool IsConnected() { return mIsConnected.load(); }


};

