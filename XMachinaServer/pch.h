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
#include "Contents/Util/Common.h"


/* Default */

#include "Resource.h"
#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

/* C++ */

#include <cassert>
#include <string>

/* STL */
#include <vector>
#include <array>
#include <stack>

/* Concurrent */
#include <concurrent_queue.h>
#include <concurrent_priority_queue.h>
#include <concurrent_unordered_map.h>

/* My Utility Hedaers */
#include "ServerLib/UtilityMacro.h"
#include "Contents/LogManager.h"


using SPtr_GameSession = std::shared_ptr<class GameSession>;
using SPtr_GameRoom    = std::shared_ptr<class GameRoom>;
using SPtr_GamePlayer  = std::shared_ptr<class GamePlayer>;


