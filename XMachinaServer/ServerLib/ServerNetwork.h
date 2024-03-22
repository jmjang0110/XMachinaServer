#pragma once

/// +-----------------------------------------------
///					   ServerNetwork 
/// ________________________________________________

/// -----------------------------------------------+

#include "NetworkInterface.h"

class ServerNetwork : public NetworkInterface
{

private:
	SPtr_Listener mListener = {};

public:
	ServerNetwork(std::wstring ip, UINT16 portNum);
	virtual ~ServerNetwork();

	virtual bool Start() override;
	virtual void Close() override;
	
public:
	SPtr_Listener GetListener() { return mListener; }


};

