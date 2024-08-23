#pragma once

/// +-----------------------------------------------
///					 NetworkObject 
/// ________________________________________________
///	±¸Á¶	
///					 NetworkObject
///						  ¡é
///					   session
///					   Listener
/// -----------------------------------------------+

#include "SocketData.h"

class NetworkObject : public std::enable_shared_from_this<NetworkObject>
{
protected:
	UINT32		     mID		  = {};
	std::string		 mName        = {};
protected:
	class SocketData mSocketData  = {};


public:
	NetworkObject();
	NetworkObject(UINT32 id, std::string name = "");
	virtual ~NetworkObject();

public:
	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) abstract;

public:
	HANDLE		GetSocketHandle()						{ return reinterpret_cast<HANDLE>(mSocketData.GetSocket()); }
	SocketData& GetSocketData()							{ return mSocketData; }
	UINT32		GetID()									{ return mID; }
	std::string GetName()								{ return mName; }

	void		SetSocketData(SocketData& sockData)		{ mSocketData = sockData; }
	void		SetName(std::string name)				{ mName       = name; }
	void		SetID(UINT32 id)						{ mID         = id; }


public:
	void PQCS(OverlappedObject* over); // PostQueuedCompletionStatus()

};

