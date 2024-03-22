#pragma once

/// +-----------------------------------------------
///					   Listener 
/// ________________________________________________

/// -----------------------------------------------+

#include "NetworkObject.h"

class Listener : public NetworkObject
{
private:
	class SocketData				mListenSocket = {};
	std::vector<Overlapped_Accept*> mAccepts	  = {};
	SPtr_NI							mOwnerNI      = {}; // Owner Network Interface 

private:
	void Register_OverlappedIO_Accept(Overlapped_Accept* overlapped);
	void Process_OverlappedIO_Accept(Overlapped_Accept* overlapped);


public:
	Listener() = default;
	virtual ~Listener();

public:
	bool Start(SPtr_NI netInterface);
	void Close();

	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;

};