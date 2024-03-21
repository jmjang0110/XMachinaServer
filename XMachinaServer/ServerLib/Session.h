#pragma once

/// +-----------------------------------------------
///					   Session 
/// ________________________________________________

/// ----------------------------------------------+


#include "SocketData.h"
#include "OverlappedObject.h"

class Session : std::enable_shared_from_this<Session>
{
private:
	/* Socket Data */
	SocketData				mSocketData = {};

	/* Overlapped I/O Object */
	Overlapped_Accept		mAccept     = {};
	Overlapped_DisConnect	mDisconnect = {};
	Overlapped_Recv			mRecv       = {};
	Overlapped_Send			mSend       = {};

	/* Packet Buffer */

public:
	Session();
	virtual ~Session();

public:
	HANDLE GetSocketHandle() { return reinterpret_cast<HANDLE>(mSocketData.GetSocket()); }

public:
	void RegisterIO(OverlappedIO::Type IoType);
	void ProcessIO(OverlappedIO::Type IoType);

};

