#pragma once
#include <thread>
#include <functional>
/// +-----------------------------------------------
///		     Thread Local Storage Manager 
/// ________________________________________________
///					Tls 괸리자 
/// Tls		 : Thread Local Storage struct 
/// MSDN	 : ( Tls Info ) https://learn.microsoft.com/ko-kr/cpp/parallel/thread-local-storage-tls?view=msvc-170
/// TlsMgr	 : Thread Local Storage Manager 
/// ----------------------------------------------+

/* TLS Data */
namespace TLS {
	
	enum class MutexInfo : UINT8 {
		TlsInfoData,
		end,
	};

	struct TlsInfoData {
		int			id;
		std::string threadName;
	};

}

/* TLS - Thread Local Storage */
#define TLS_MGR TlsMgr::GetInst()
using TlsMgr = class ThreadLocalStorageManager;
class ThreadLocalStorageManager 
{
	DECLARE_SINGLETON(TlsMgr);
private:
	DWORD mTlsIndex;
public:
	static std::atomic<UINT32> NewThreadID; // TLS에 Id 를 부여하기 위한 atomic 변수 
	std::mutex mMutexArr[static_cast<UINT8>(TLS::MutexInfo::end)];

public:
	ThreadLocalStorageManager();
	~ThreadLocalStorageManager();

public:
	/* TLS::TlsInfoData */
	void Init_TlsInfoData(std::string threadName);
	TLS::TlsInfoData* Get_TlsInfoData();

};


/// +-----------------------------------------------
///					Thread Manager 
/// ________________________________________________
///				전체 쓰레드 관리자
/// Tls		 : Thread Local Storage struct 
/// MSDN	 : ( Tls Info ) https://learn.microsoft.com/ko-kr/cpp/parallel/thread-local-storage-tls?view=msvc-170
/// TlsMgr	 : Thread Local Storage Manager 
/// ----------------------------------------------+
/* Thread Manager */
#define THREAD_MGR ThreadManager::GetInst()
class ThreadManager
{
	DECLARE_SINGLETON(ThreadManager);

private:
	std::mutex					mLock    = {};
	std::vector<std::thread>	mThreads = {};

public:
	ThreadManager();
	~ThreadManager();

public:
	void RunThread(std::string threadName, std::function<void(void)> func);
	void JoinAllThreads();
};

