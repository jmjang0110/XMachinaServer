#include "pch.h"
#include "MemoryManager.h"


DEFINE_SINGLETON(MemoryManager);
MemoryManager::MemoryManager()
{
}

MemoryManager::~MemoryManager()
{
	for (auto Pool : mSListMemoryPools) {
		SAFE_DELETE(Pool);
	}
	mSListMemoryPools.clear();
}

void MemoryManager::AddSListMemoryPool(std::string mpName, size_t MemorySize, size_t numMemoryBlocks)
{
    /* Locking */
    while (mAtomicFlag.test_and_set(std::memory_order_acquire))
    {
    };
    
    if (mSListMemoryPoolsDict[mpName] == nullptr) {
        SListMemoryPool* Pool = new SListMemoryPool(MemorySize , numMemoryBlocks);
        mSListMemoryPools.push_back(Pool);
        mSListMemoryPoolsDict[mpName] = Pool;
    }
    // 해당 메모리 사이즈를 갖는 풀이 이미 존재한다면 
    else {
        mSListMemoryPoolsDict[mpName]->AddMemory();
    }

    /* UnLocking */
    mAtomicFlag.clear(std::memory_order_release); 
}

void* MemoryManager::Allocate(size_t size)
{
    while (mAtomicFlag.test_and_set(std::memory_order_acquire)); // Locking
    void* ptr = nullptr;
    // Find a suitable memory pool
    for (auto pool : mSListMemoryPools) {
        if (size <= pool->GetMemorySize()) {
            ptr = pool->Pull();
            break;
        }
    }
    mAtomicFlag.clear(std::memory_order_release); // Unlocking
    return ptr;
}

void MemoryManager::Free(std::string mpName, void* ptr)
{
    /* Locking */
    while (mAtomicFlag.test_and_set(std::memory_order_acquire))
    {
    };

    if (mSListMemoryPoolsDict[mpName] != nullptr) {
        mSListMemoryPoolsDict[mpName]->Push(ptr);
    }

    /* UnLocking */
    mAtomicFlag.clear(std::memory_order_release);
}
