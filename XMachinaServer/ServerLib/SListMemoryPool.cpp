#include "pch.h"
#include "SListMemoryPool.h"
/*
Concurrency_Queue 
*/

SListMemoryPool::SListMemoryPool(size_t MemorySize, size_t numBlocks) 
    : mMemorySize(MemorySize)
{
    mNumBlocks.store(numBlocks);

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

}

void* SListMemoryPool::Pull() 
{
    mNumBlocks.fetch_sub(1);

    // SLIST에서 메모리 블록 가져오기
    PSLIST_ENTRY entry = ::InterlockedPopEntrySList(&mSListHeader);
  

    if (entry) {
        return reinterpret_cast<void*>(entry);
    }
    return nullptr; // 사용 가능한 블록이 없는 경우
}

void SListMemoryPool::Push(void* ptr)
{
    mNumBlocks.fetch_add(1);

    // SLIST에 메모리 블록 추가
    ::InterlockedPushEntrySList(&mSListHeader, reinterpret_cast<PSLIST_ENTRY>(ptr));
}

void SListMemoryPool::AddMemory()
{

    // SLIST에 메모리 블록 추가
    SListMemoryBlock* ptr = static_cast<SListMemoryBlock*>(::_aligned_malloc(sizeof(SListMemoryBlock) + mMemorySize, MEMORY_ALLOCATION_ALIGNMENT));
    if (ptr) {
        mNumBlocks.fetch_add(1);
        InterlockedPushEntrySList(&mSListHeader, reinterpret_cast<PSLIST_ENTRY>(ptr));
    }

}
