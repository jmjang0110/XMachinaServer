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
		
		/* Init Thread Local Storage */
		TLS_MGR->Init_TlsInfoData(threadName);
		
		
		func();
		
		/* Destroy Thread Local Storage */
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
	for (UINT8 i = 0; i < static_cast<UINT8>(TLS::TlsIndex::end); ++i) {
		mTlsIndex[i] = ::TlsAlloc();
	}
}

ThreadLocalStorageManager::~ThreadLocalStorageManager()
{
	for (UINT8 i = 0; i < static_cast<UINT8>(TLS::TlsIndex::end); ++i) {
		::TlsFree(mTlsIndex[i]);
	}


}

bool ThreadLocalStorageManager::Init()
{
	return true;

}


/* TLS::TlsInfoData */
void ThreadLocalStorageManager::Init_TlsInfoData(std::string threadName)
{
	const UINT8 TlsIndexIdx = static_cast<UINT8>(TLS::TlsIndex::TlsInfoData);

	/// TLS ������ ���� 
	TLS::TlsInfoData* TlsData = new TLS::TlsInfoData;
	TlsData->id               = TlsMgr::NewThreadID.fetch_add(1); // atomic 
	TlsData->threadName       = threadName;

	/* lock! */
	std::lock_guard<std::mutex> lock(mMutexArr[TlsIndexIdx]);
	::TlsSetValue(mTlsIndex[TlsIndexIdx], TlsData);
}

TLS::TlsInfoData* ThreadLocalStorageManager::Get_TlsInfoData()
{
	// ���� �������� TLS �����͸� �����´�. 
	//std::lock_guard<std::mutex> lock(mMutexArr[static_cast<UINT8>(TLS::MutexInfo::TlsInfoData)]);

	const UINT8 TlsIndexIdx = static_cast<UINT8>(TLS::TlsIndex::TlsInfoData);
	return static_cast<TLS::TlsInfoData*>(::TlsGetValue(mTlsIndex[TlsIndexIdx]));
}

#pragma endregion