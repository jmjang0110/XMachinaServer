#pragma once

/// +-----------------------------------------------
///					ClientNetwork 
/// ________________________________________________

/// -----------------------------------------------+

#include "NetworkInterface.h"

class ClientNetwork : public NetworkInterface
{
private:

public:
	ClientNetwork(std::wstring ip, UINT32 PortNum);
	virtual ~ClientNetwork();

	virtual bool Start() override;
	virtual void Close() override;
};

