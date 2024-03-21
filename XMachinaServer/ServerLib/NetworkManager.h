#pragma once

/// +-----------------------------------------------
///					Network Manager 
/// ________________________________________________
///				  네트워크 기능 관리자 
/// 
/// ----------------------------------------------+

#define NETWORK_MGR NetworkManager::GetInst()
class NetworkManager
{
	DECLARE_SINGLETON(NetworkManager);
private:
	LPFN_CONNECTEX		mLpfn_ConnectEx    = {};
	LPFN_DISCONNECTEX	mLpfn_DisconnectEx = {};
	LPFN_ACCEPTEX		mLpfn_AcceptEx     = {};

public:
	NetworkManager();
	~NetworkManager();

public:
	bool Init();

public:
	LPFN_CONNECTEX		ConnectEx()		{ return mLpfn_ConnectEx; }
	LPFN_DISCONNECTEX	DiscconectEx()	{ return mLpfn_DisconnectEx; }
	LPFN_ACCEPTEX		AcceptEx()		{ return mLpfn_AcceptEx; }

public:
	IN_ADDR	Ip2Address(const WCHAR* ip);

};

