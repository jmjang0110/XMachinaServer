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
    mPushCount.store(0);
    mPullCount.store(0);

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
    // SLIST에서 메모리 블록 가져오기
    //mNumBlocks.fetch_sub(1);
    PSLIST_ENTRY ptr = ::InterlockedPopEntrySList(&mSListHeader);

   // mPullCount.fetch_add(1);
    //std::cout << "Pull Count : " << mPullCount.load() << std::endl;
    
    if (ptr) {
        //LOG_MGR->Cout("GetFrom Memory Pool\n");

        //if (TLS_MGR->Get_TlsInfoData()) {
        //    if (TLS_MGR->Get_TlsInfoData()->id) {
             //  std::cout << TLS_MGR->Get_TlsInfoData()->id << " thread[PULL] " << " Memory - " << reinterpret_cast<PSLIST_ENTRY>(reinterpret_cast<char*>(ptr) + sizeof(SLIST_ENTRY)) << std::endl;

        //    }
       // }


        /* ptr-------->ptr                  */       
        /* ↓            ↓                   */       
        /* [SLIST_ENTRY][ ... memory ... ]  */
        return reinterpret_cast<void*>(reinterpret_cast<char*>(ptr) + sizeof(SLIST_ENTRY));
    }
    else {
       // LOG_MGR->Cout("Create New Memory \n");

        /* 없으면 만들어서.. */
        const size_t AllocSizse = sizeof(SLIST_ENTRY) + mMemorySize;
        SListMemoryBlock* AddPtr = reinterpret_cast<SListMemoryBlock*>(::_aligned_malloc(AllocSizse, MEMORY_ALLOCATION_ALIGNMENT));
        if (AddPtr != nullptr) {

            /*mNumBlocks.fetch_add(1);
            ::InterlockedPushEntrySList(&mSListHeader, reinterpret_cast<PSLIST_ENTRY>(AddPtr));
            ptr = ::InterlockedPopEntrySList(&mSListHeader);
            if (ptr == nullptr) {

            }
            */  
            return reinterpret_cast<void*>(reinterpret_cast<char*>(AddPtr) + sizeof(SLIST_ENTRY));
        }
        else {
            return nullptr;
        }

       // if (TLS_MGR->Get_TlsInfoData()) {

       //     if (TLS_MGR->Get_TlsInfoData()->id) {
                // std::cout << TLS_MGR->Get_TlsInfoData()->id << " Thread - " << "Push..." << reinterpret_cast<void*>(reinterpret_cast<char*>(ptr) + sizeof(SLIST_ENTRY)) << std::endl;
                // std::cout << "Push count : " << mPushCount.load() << std::endl;
                //std::cout << TLS_MGR->Get_TlsInfoData()->id << " thread[PULL] "  << " Memory - " << reinterpret_cast<PSLIST_ENTRY>(reinterpret_cast<char*>(ptr) + sizeof(SLIST_ENTRY)) << std::endl;

       // //    }
       // }

    }

    return nullptr; // 사용 가능한 블록이 없는 경우 + 블록을 새로 만들기도 실패한 경우 
}

void SListMemoryPool::Push(void* ptr)
{
   // mPushCount.fetch_add(1);
   // mNumBlocks.fetch_add(1);

    //if (TLS_MGR->Get_TlsInfoData()->id) {
       // std::cout << TLS_MGR->Get_TlsInfoData()->id << " thread[PUSH] " << " Memory - " << reinterpret_cast<PSLIST_ENTRY>(reinterpret_cast<char*>(ptr) ) << std::endl;
    //}

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

    const size_t AllocSizse = sizeof(SLIST_ENTRY) + mMemorySize;
    SListMemoryBlock* ptr = reinterpret_cast<SListMemoryBlock*>(::_aligned_malloc(AllocSizse, MEMORY_ALLOCATION_ALIGNMENT));
    if (ptr) {

        mNumBlocks.fetch_add(1);
        ::InterlockedPushEntrySList(&mSListHeader, reinterpret_cast<PSLIST_ENTRY>(ptr));
    }

}
