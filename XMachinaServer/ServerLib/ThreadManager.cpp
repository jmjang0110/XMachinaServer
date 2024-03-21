#include "pch.h"
#include "ThreadManager.h"

/// +----------------------------------
///			  Thread Manager 
/// ----------------------------------+
#pragma region Thread Manager 
DEFINE_SINGLETON(ThreadManager);

ThreadManager::ThreadManager()
{

}

ThreadManager::~ThreadManager()
{
	TLS_MGR->Destroy();
}
void ThreadManager::RunThread(std::string threadName, std::function<void(void)> func)
{
	std::lock_guard<std::mutex> lockGuard(mLock);

	mThreads.push_back(std::thread([=]() {
		
		TLS_MGR->Init_TlsInfoData(threadName);
		func();
		auto tlsDataPtr = TLS_MGR->Get_TlsInfoData();
		SAFE_DELETE(tlsDataPtr);
		
		}));

}
void ThreadManager::JoinAllThreads()
{
	/* Join All threads */
	for (std::thread& thread : mThreads)
	{
		if (thread.joinable())
			thread.join(); 
	}
	mThreads.clear();
}
#pragma endregion


/// +----------------------------------
///		Thread Local Storage Manager 
/// ----------------------------------+
#pragma region Thread Local Storage Manager 
/* static value Init */
std::atomic<UINT32> TlsMgr::NewThreadID = 1;
DEFINE_SINGLETON(ThreadLocalStorageManager);

ThreadLocalStorageManager::ThreadLocalStorageManager()
{
	mTlsIndex = ::TlsAlloc();
}

ThreadLocalStorageManager::~ThreadLocalStorageManager()
{
	::TlsFree(mTlsIndex);
}

/* TLS::TlsInfoData */
void ThreadLocalStorageManager::Init_TlsInfoData(std::string threadName)
{
	/// TLS 데이터 생성 
	TLS::TlsInfoData* TlsData = new TLS::TlsInfoData;
	TlsData->id               = TlsMgr::NewThreadID.fetch_add(1);
	TlsData->threadName       = threadName;

	/* lock! */
	std::lock_guard<std::mutex> lock(mMutexArr[static_cast<UINT8>(TLS::MutexInfo::TlsInfoData)]);
	::TlsSetValue(mTlsIndex, TlsData);
}

TLS::TlsInfoData* ThreadLocalStorageManager::Get_TlsInfoData()
{
	// 현재 쓰레드의 TLS 데이터를 가져온다. 
	//std::lock_guard<std::mutex> lock(mMutexArr[static_cast<UINT8>(TLS::MutexInfo::TlsInfoData)]);
	return static_cast<TLS::TlsInfoData*>(::TlsGetValue(mTlsIndex));
}
#pragma endregion