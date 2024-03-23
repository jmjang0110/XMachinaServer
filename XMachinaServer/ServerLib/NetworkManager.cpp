#include "pch.h"
#include "NetworkManager.h"
#include "OverlappedObject.h"


DEFINE_SINGLETON(NetworkManager);

NetworkManager::NetworkManager()
{

}

NetworkManager::~NetworkManager()
{
	::WSACleanup();
}

bool NetworkManager::Init()
{
	/* WSAStartUp */
	if (FALSE == NetworkManager::WSAStartUp(2, 2))
		return false;


	/* LPFN 초기화 */
	SocketData dummySocketData = {};
	dummySocketData.CreateSocket();
	dummySocketData.BindWindowsFunction(WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&mLpfn_ConnectEx));
	dummySocketData.BindWindowsFunction(WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&mLpfn_DisconnectEx));
	dummySocketData.BindWindowsFunction(WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&mLpfn_AcceptEx));
	dummySocketData.Close();
	if (mLpfn_AcceptEx == nullptr || mLpfn_DisconnectEx == nullptr || mLpfn_ConnectEx == nullptr)
		return false;

	return true;
}

bool NetworkManager::WSAStartUp(INT32 major, INT32 minor)
{
	WSADATA wsaData = {};
	int errCode = ::WSAStartup(MAKEWORD(major, minor), OUT & wsaData);
	if (errCode != 0) {
		std::cout << "::WSAStartUp Error : " << errCode << std::endl;
		return false;
	}
	std::cout << wsaData.wVersion << wsaData.wHighVersion << std::endl;
}


IN_ADDR NetworkManager::Ip2Address(const WCHAR* ip)
{
	IN_ADDR address{};
	::InetPtonW(AF_INET, ip, &address); // InetPtoN : Pointer(문자열) to Network 
	return address;
}

