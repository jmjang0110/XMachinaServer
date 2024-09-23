#pragma once

//#define SET_DATA_FROM_DATABASE

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
#include <chrono>
#include <cstdint> 

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

/* SQL */
#include <sql.h>
#include <sqlext.h>


template<typename T>
using SPtr = std::shared_ptr<T>;

/* My Utility Hedaers */
#undef max
#include "Enum_generated.h"
#include "ServerLib/UtilityMacro.h"
#include "Contents/LogManager.h"
#include "Contents/TimeManager.h"
#include "ServerLib/MemoryManager.h"
#include "Contents/Util/Common.h"
#include "Contents/EntityTag.h"
#include "Contents/ScriptKey.h"
#include "Contents/ObjectTag.h"
#include "ServerLib/NetworkObject.h"



constexpr UINT32 MAX_SESSION_NUM = 50;

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
	// Check if either x or z is negative
	bool checkNegative() const {
		return x < 0 || z < 0;
	}
};


struct Atomic_Vec3
{
	std::atomic<float> x;
	std::atomic<float> y;
	std::atomic<float> z;

};


template<typename T>
class SRWL_Data
{
private:
	Lock::SRWLock _lock;
	T _data;

public:
	void SetData(T& data) {
		_lock.LockWrite();
		T ret = data;
		_lock.UnlockWrite();
	}

	T GetData() {
		_lock.LockRead();
		T ret = _data;
		_lock.UnlockRead();
		return ret;
	}
};
