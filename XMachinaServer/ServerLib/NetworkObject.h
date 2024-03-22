#pragma once


/// +-----------------------------------------------
///					 NetworkObject 
/// ________________________________________________
///	����	
///					 NetworkObject
///						  ��
///					   session
///					   Listener
/// ----------------------------------------------+


class NetworkObject : public std::enable_shared_from_this<NetworkObject>
{
private:
	class SocketData mSocketData = {};

public:
	NetworkObject();
	virtual ~NetworkObject();

public:
	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) abstract;

public:
	HANDLE		GetSocketHandle()	{ return reinterpret_cast<HANDLE>(mSocketData.GetSocket()); }
	SocketData& GetSocketData()		{ return mSocketData; }

	void		SetSocketData(SocketData& sockData) { mSocketData = sockData; }

};

