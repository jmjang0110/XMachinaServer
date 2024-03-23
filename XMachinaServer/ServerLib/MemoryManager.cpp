#include "pch.h"
#include "MemoryManager.h"


DEFINE_SINGLETON(MemoryManager);
MemoryManager::MemoryManager()
{
}

MemoryManager::~MemoryManager()
{
	for (auto Pool : mMemoryPools) {
		SAFE_DELETE(Pool);
	}
	mMemoryPools.clear();
}

void MemoryManager::AddMemoryPool(size_t memorySize, size_t numMemoryBlocks)
{
    /* Locking */
    while (mAtomicFlag.test_and_set(std::memory_order_acquire))
    {
    };
    
    if (mMemoryPoolsDict[memorySize] == nullptr) {
        MemoryPool* Pool = new MemoryPool(memorySize, numMemoryBlocks);
        mMemoryPools.push_back(Pool);
        mMemoryPoolsDict[memorySize] = Pool;
    }
    // 해당 메모리 사이즈를 갖는 풀이 이미 존재한다면 
    else {
        mMemoryPoolsDict[memorySize]->AddMemory();
    }

    /* UnLocking */
    mAtomicFlag.clear(std::memory_order_release); 
}

void* MemoryManager::Allocate(size_t size)
{
    while (mAtomicFlag.test_and_set(std::memory_order_acquire)); // Locking
    void* ptr = nullptr;
    // Find a suitable memory pool
    for (auto pool : mMemoryPools) {
        if (size <= pool->GetBlockSize()) {
            ptr = pool->Allocate();
            break;
        }
    }
    mAtomicFlag.clear(std::memory_order_release); // Unlocking
    return ptr;
}

void MemoryManager::Free(size_t memorySize, void* ptr)
{
    /* Locking */
    while (mAtomicFlag.test_and_set(std::memory_order_acquire))
    {
    };

    if (mMemoryPoolsDict[memorySize] != nullptr) {
        mMemoryPoolsDict[memorySize]->Free(ptr);
    }

    /* UnLocking */
    mAtomicFlag.clear(std::memory_order_release);
}
