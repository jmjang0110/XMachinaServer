#include "pch.h"
#include "MemoryPool.h"

MemoryPool::MemoryPool(size_t MemorySize, size_t numBlocks) 
    : mBlockSize(MemorySize), mMemorySize(MemorySize)
{
    // SLIST 헤더 초기화
    ::InitializeSListHead(&mSListHeader);
    AddMemory();
}



MemoryPool::~MemoryPool()
{
    // 메모리 블록들을 해제
    PSLIST_ENTRY entry;
    while ((entry = ::InterlockedPopEntrySList(&mSListHeader)) != nullptr) {
        MemoryBlock* ptr = reinterpret_cast<MemoryBlock*>(entry);
        _aligned_free(ptr);
    }
}

void* MemoryPool::Allocate() 
{
    // SLIST에서 메모리 블록 가져오기
    PSLIST_ENTRY entry = ::InterlockedPopEntrySList(&mSListHeader);

    if (entry) {
        return reinterpret_cast<void*>(entry);
    }
    return nullptr; // 사용 가능한 블록이 없는 경우
}

void MemoryPool::Free(void* ptr)
{
    // SLIST에 메모리 블록 추가
    ::InterlockedPushEntrySList(&mSListHeader, reinterpret_cast<PSLIST_ENTRY>(ptr));
}

void MemoryPool::AddMemory()
{
    // SLIST에 메모리 블록 추가
    MemoryBlock* ptr = static_cast<MemoryBlock*>(::_aligned_malloc(sizeof(MemoryBlock) + mMemorySize, MEMORY_ALLOCATION_ALIGNMENT));
    if (ptr) {
        InterlockedPushEntrySList(&mSListHeader, reinterpret_cast<PSLIST_ENTRY>(ptr));
    }

}
