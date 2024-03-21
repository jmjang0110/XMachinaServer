#include "pch.h"
#include "MemoryPool.h"

MemoryPool::MemoryPool(size_t MemorySize, size_t numBlocks) 
    : mBlockSize(MemorySize)
{
    // SLIST 헤더 초기화
    InitializeSListHead(&mSListHeader);

    // 메모리 블록을 미리 할당하여 SLIST에 추가
    for (size_t i = 0; i < numBlocks; ++i) {
        MemoryBlock* ptr = static_cast<MemoryBlock*>(_aligned_malloc(sizeof(MemoryBlock) + MemorySize, MEMORY_ALLOCATION_ALIGNMENT));
        
        if (ptr == nullptr) {
            // 할당 실패 시, 이전에 할당한 메모리 해제
            for (size_t j = 0; j < i; ++j) {
                _aligned_free(mMemoryBlocks[j]);
            }
        }

        InterlockedPushEntrySList(&mSListHeader, reinterpret_cast<PSLIST_ENTRY>(ptr));
        mMemoryBlocks.push_back(ptr); // 메모리 블록을 추적하기 위해 벡터에 추가
    }
}


MemoryPool::~MemoryPool()
{
    // 할당된 메모리 블록들 해제
    for (auto ptr : mMemoryBlocks) {
        _aligned_free(ptr);
    }
}

void* MemoryPool::Allocate() 
{
    // SLIST에서 메모리 블록 가져오기
    PSLIST_ENTRY entry = InterlockedPopEntrySList(&mSListHeader);

    if (entry) {
        return reinterpret_cast<void*>(entry);
    }
    return nullptr; // 사용 가능한 블록이 없는 경우
}

void MemoryPool::Free(void* ptr)
{
    // SLIST에 메모리 블록 추가
    InterlockedPushEntrySList(&mSListHeader, reinterpret_cast<PSLIST_ENTRY>(ptr));
}
