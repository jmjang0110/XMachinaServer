#include "pch.h"
#include "NetworkManager.h"

DEFINE_SINGLETON(NetworkManager);

NetworkManager::NetworkManager()
{
	WSADATA wsaData;
	assert(::WSAStartup(MAKEWORD(2, 2), OUT & wsaData) == 0);


}

NetworkManager::~NetworkManager()
{
	::WSACleanup();
}

bool NetworkManager::Init(SocketData socket)
{
	/* LPFN �ʱ�ȭ */
	SocketData dummy;
	dummy.CreateSocket();
	dummy.BindWindowsFunction(WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&mLpfn_ConnectEx));
	dummy.BindWindowsFunction(WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&mLpfn_DisconnectEx));
	dummy.BindWindowsFunction(WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&mLpfn_AcceptEx));
	dummy.Close();

	return true;
}

IN_ADDR NetworkManager::Ip2Address(const WCHAR* ip)
{
	IN_ADDR address{};
	::InetPtonW(AF_INET, ip, &address); // InetPtoN : Pointer(���ڿ�) to Network 
	return address;
}

