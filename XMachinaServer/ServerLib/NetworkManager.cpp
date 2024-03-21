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

bool NetworkManager::Init()
{

	return true;
}

IN_ADDR NetworkManager::Ip2Address(const WCHAR* ip)
{
	IN_ADDR address{};
	::InetPtonW(AF_INET, ip, &address); // InetPtoN : Pointer(¹®ÀÚ¿­) to Network 
	return address;
}

