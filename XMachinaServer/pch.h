#pragma once

#include "ServerLib/NetworkHeaders.h"
#include "Contents/Util/Common.h"
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
#include <numeric>
#include <filesystem>

/* STL */
#include <vector>
#include <array>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <ranges>


/* Concurrent */
#include <concurrent_queue.h>
#include <concurrent_priority_queue.h>
#include <concurrent_unordered_map.h>
#include <concurrent_unordered_set.h>
#include <atomic>

/* My Utility Hedaers */
#include "ServerLib/UtilityMacro.h"
#include "Contents/LogManager.h"
#include "Contents/TimeManager.h"
#include "ServerLib/MemoryManager.h"
#include "Contents/Util/Common.h"
#include "Contents/Gameinfo.h"

template<typename T>
using SPtr = std::shared_ptr<T>;


using SPtr_GameSession   = std::shared_ptr<class GameSession>;
using SPtr_GameRoom      = std::shared_ptr<class GameRoom>;
using SPtr_GamePlayer    = std::shared_ptr<class GamePlayer>;
using SPtr_GameMonster   = std::shared_ptr<class GameMonster>;
using SPtr_GameStructure = std::shared_ptr<class GameStructure>;
using SPtr_GameBullet    = std::shared_ptr<class GameBullet>;
using SPtr_GameObject    = std::shared_ptr<class GameObject>;


constexpr UINT32 MAX_SESSION_NUM = 500;

/* 좌표 */
struct Coordinate
{
	int x = -1;
	int z = -1;

	// Overloading the != operator
	bool operator!=(const Coordinate& other) const {
		return x != other.x || z != other.z;
	}
	bool operator==(const Coordinate& other) const {
		return x == other.x || z == other.z;
	}
};


struct Atomic_Vec3
{
	std::atomic<float> x;
	std::atomic<float> y;
	std::atomic<float> z;

};


