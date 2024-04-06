#include "pch.h"
#include "SListMemoryPool.h"
#include "ThreadManager.h"

/*
Concurrency_Queue 
*/

SListMemoryPool::SListMemoryPool(size_t MemorySize) 
    : mMemorySize(MemorySize)
{
    mNumBlocks.store(0);
    // SLIST 헤더 초기화
    ::InitializeSListHead(&mSListHeader);
     AddMemory();
}



SListMemoryPool::~SListMemoryPool()
{
    // 메모리 블록들을 해제
    PSLIST_ENTRY entry;
    while ((entry = ::InterlockedPopEntrySList(&mSListHeader)) != nullptr) {
        SListMemoryBlock* ptr = reinterpret_cast<SListMemoryBlock*>(entry);
        _aligned_free(ptr);
    }
    mNumBlocks.store(0);

    mPushCount.store(0);
    mPullCount.store(0);

}

void* SListMemoryPool::Pull() 
{
    //if (mNumBlocks.load() == 0)
     //   return nullptr;



    // SLIST에서 메모리 블록 가져오기
    mNumBlocks.fetch_sub(1);
    PSLIST_ENTRY ptr = ::InterlockedPopEntrySList(&mSListHeader);

    if (TLS_MGR->Get_TlsInfoData()) {
       
       //std::cout  << TLS_MGR->Get_TlsSendBufFactory()->SendBufFactory << " 발급 아이디 : " << TLS_MGR->Get_TlsInfoData()->id << " 메모리 포인터 - " << reinterpret_cast<void*>(reinterpret_cast<char*>(ptr) + sizeof(SLIST_ENTRY)) << " "  << mNumBlocks.load() << std::endl;
    }

    mPullCount.fetch_add(1);



    if (ptr) {
        /* ptr-------->ptr                  */       
        /* ↓            ↓                   */       
        /* [SLIST_ENTRY][ ... memory ... ]  */
        return reinterpret_cast<void*>(reinterpret_cast<char*>(ptr) + sizeof(SLIST_ENTRY));
    }
    else {
        /* 없으면 만들어서.. */
        AddMemory();
        mNumBlocks.fetch_sub(1);
        PSLIST_ENTRY ptr = ::InterlockedPopEntrySList(&mSListHeader);
        return reinterpret_cast<void*>(reinterpret_cast<char*>(ptr) + sizeof(SLIST_ENTRY));

    }


    return nullptr; // 사용 가능한 블록이 없는 경우
}

void SListMemoryPool::Push(void* ptr)
{
    mPushCount.fetch_add(1);
    //std::cout << TLS_MGR->Get_TlsSendBufFactory()->SendBufFactory << "반납 아이디 : " << TLS_MGR->Get_TlsInfoData()->id << " 메모리 포인터 - " << ptr << " PUSHCNT : " << mPushCount.load() << std::endl;

    mNumBlocks.fetch_add(1);
    //std::cout << TLS_MGR->Get_TlsSendBufFactory()->SendBufFactory << "반납 아이디 : " << TLS_MGR->Get_TlsInfoData()->id << " 메모리 포인터 - "  << ptr << " 갯수 : " << mNumBlocks.load() << std::endl;


    // SLIST에 메모리 블록 추가
        /* ptr<--------ptr                  */       
        /* ↓            ↓                   */       
        /* [SLIST_ENTRY][ ... memory ... ]  */
    ::InterlockedPushEntrySList(&mSListHeader, reinterpret_cast<PSLIST_ENTRY>(reinterpret_cast<char*>(ptr) - sizeof(SLIST_ENTRY)));
}

void SListMemoryPool::AddMemory()
{
    // SLIST에 메모리 블록 추가
    /* [SLIST_ENTRY][ ... memory ... ] */
    SListMemoryBlock* ptr = static_cast<SListMemoryBlock*>(::_aligned_malloc(sizeof(SLIST_ENTRY) + mMemorySize, MEMORY_ALLOCATION_ALIGNMENT));
    if (ptr) {
        mNumBlocks.fetch_add(1);
        InterlockedPushEntrySList(&mSListHeader, reinterpret_cast<PSLIST_ENTRY>(ptr));
    }

}
