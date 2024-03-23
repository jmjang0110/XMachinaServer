
/// +-----------------------------------
///		   네트워크 전용 헤더 파일 
/// -----------------------------------+


/* Winsock */
#pragma region WinSock - Network
#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

/* Network */
//#include <iphlpapi.h>				
//#pragma comment(lib, "iphlpapi.lib")
#pragma endregion

#include <malloc.h> 
#include <memory.h>
#include <functional>

/* vector */
#include <vector>
#include <queue>
#include <unordered_map>

