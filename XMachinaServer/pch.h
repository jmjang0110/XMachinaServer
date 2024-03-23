#pragma once

#include "ServerLib/NetworkHeaders.h"

/// +------------------------
///			COMMON
/// ------------------------+
#pragma region C++ Library
#include <windows.h>
#include <iostream>
#include <functional>
#include <cassert>
#pragma endregion


/* Default */

#include "Resource.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

/* C++ */

#include <cassert>
#include <string>

/* STL */
#include <vector>


/* My Utility Hedaers */
#include "ServerLib/UtilityMacro.h"
#include "Contents/LogManager.h"

/* Shared ptr */
using SPtr_Session       = std::shared_ptr<class Session>;
using SPtr_SendPktBuf    = std::shared_ptr<class PacketSendBuf>;
using SPtr_RecvPktBuf    = std::shared_ptr<class PacketRecvBuf>;
using SPtr_NetObj        = std::shared_ptr<class NetworkObject>;
using SPtr_Listener      = std::shared_ptr<class Listener>;
using SPtr_NI            = std::shared_ptr<class NetworkInterface>;
using SPtr_ServerNetwork = std::shared_ptr<class ServerNetwork>;
using SPtr_ClientNetwork = std::shared_ptr<class ClientNetwork>;

#include "ServerLib/Session.h"