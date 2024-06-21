#pragma once


#include <WinSock2.h>
#include <winsock.h>
#include <Windows.h>
#include <iostream>
#include <thread>
#include <vector>
#include <unordered_set>
#include <mutex>
#include <atomic>
#include <chrono>
#include <queue>
#include <array>
#include <memory>

#pragma comment (lib, "ws2_32.lib")

using namespace std::chrono;


const static int MAX_TEST        = 100000;
const static int MAX_CLIENTS     = MAX_TEST * 2;
const static int INVALID_ID      = -1;
const static int MAX_PACKET_SIZE = 255;
const static int MAX_BUFF_SIZE   = 255;


constexpr int DELAY_LIMIT = 100;
constexpr int DELAY_LIMIT2 = 150;
constexpr int ACCEPT_DELY = 50;



#pragma once

/// +----------------------------
///			 SINGLETON
/// ----------------------------+
#pragma region SINGLETON
#define DECLARE_SINGLETON(TYPE)             \
private:									\
	 static TYPE* m_pInst;					\
public:										\
	static TYPE* GetInst()					\
	{										\
		if (!m_pInst) m_pInst = new TYPE;	\
		return m_pInst;						\
	}										\
	static void Destroy() {					\
		if (nullptr != m_pInst) {			\
			delete m_pInst;					\
			m_pInst = nullptr;				\
		}									\
	}						

#define DEFINE_SINGLETON(TYPE)  \
	 TYPE* TYPE::m_pInst = nullptr;
#pragma endregion

#pragma region Delete
#define SAFE_DELETE(ptr) \
if(ptr != nullptr){		 \
	delete ptr;			 \
	ptr = nullptr;		 \
}	
#pragma endregion

