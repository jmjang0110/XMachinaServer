#include "pch.h"
#include "MemoryManager.h"


DEFINE_SINGLETON(MemoryManager);
MemoryManager::MemoryManager()
{
}

MemoryManager::~MemoryManager()
{
    /* Delete SListMemoryPool !!! */
    for (auto& entry : mSLMemPoolsDict_Name) {
        delete entry.second;
        mSLMemPoolsDict_Name[entry.first] = nullptr;

    }
    mSLMemPoolsDict_Name.clear();

    for (auto& entry : mSLMemPoolsDict_Size) {
        ServerMemoryPool* ptr = entry.second;
        delete ptr;
        mSLMemPoolsDict_Size[entry.first] = nullptr;
    }
    mSLMemPoolsDict_Size.clear();

    LOG_MGR->Cout("~MemoryManager\n");

}

bool MemoryManager::InitMemories()
{
    ///* Memory를 미리 할당해놓자! */
    AddMemoryPool(MemorySize::BYTES_32, 100000);
    AddMemoryPool(MemorySize::BYTES_64, 1000000);
    AddMemoryPool(MemorySize::BYTES_128, 100000);
    AddMemoryPool(MemorySize::BYTES_256, 100000);
    AddMemoryPool(MemorySize::BYTES_512, 100000);
    AddMemoryPool(MemorySize::BYTES_1024, 50000);
    AddMemoryPool(MemorySize::BYTES_2048, 5000);
    AddMemoryPool(MemorySize::BYTES_4096, 5000);

    return true;
}

void MemoryManager::AddMemoryPool(MemorySize memSize, size_t memoryBlockCnt)
{
    if (mSLMemPoolsDict_Size.find(memSize) != mSLMemPoolsDict_Size.end())
        return;

    mSLMemPoolsDict_Size[memSize] = new ServerMemoryPool();
    mSLMemPoolsDict_Size[memSize]->Init(static_cast<size_t>(memSize), memoryBlockCnt);

}

void MemoryManager::AddMemoryPool(std::string memName, size_t memSize, size_t memoryBlockCnt)
{
    if (mSLMemPoolsDict_Name.find(memName) != mSLMemPoolsDict_Name.end())
        return;

    mSLMemPoolsDict_Name[memName] = new ServerMemoryPool();
    mSLMemPoolsDict_Name[memName]->Init(static_cast<size_t>(memSize), memoryBlockCnt);
}


void* MemoryManager::Allocate(size_t size)
{
    void* ptr = nullptr;

    if (size <= static_cast<size_t>(MemorySize::BYTES_32)) {
        ptr = mSLMemPoolsDict_Size[MemorySize::BYTES_32]->Pull();
    }
    else if (size <= static_cast<size_t>(MemorySize::BYTES_64)) {
        ptr = mSLMemPoolsDict_Size[MemorySize::BYTES_64]->Pull();

    }
    else if (size <= static_cast<size_t>(MemorySize::BYTES_128)) {
        ptr = mSLMemPoolsDict_Size[MemorySize::BYTES_128]->Pull();

    }
    else if (size <= static_cast<size_t>(MemorySize::BYTES_256)) {
        ptr = mSLMemPoolsDict_Size[MemorySize::BYTES_256]->Pull();

    }
    else if (size <= static_cast<size_t>(MemorySize::BYTES_512)) {
        ptr = mSLMemPoolsDict_Size[MemorySize::BYTES_512]->Pull();

    }
    else if (size <= static_cast<size_t>(MemorySize::BYTES_1024)) {
        ptr = mSLMemPoolsDict_Size[MemorySize::BYTES_1024]->Pull();

    }
    else if (size <= static_cast<size_t>(MemorySize::BYTES_2048)) {
        ptr = mSLMemPoolsDict_Size[MemorySize::BYTES_2048]->Pull();
    }

    else if (size <= static_cast<size_t>(MemorySize::BYTES_4096)) {
        ptr = mSLMemPoolsDict_Size[MemorySize::BYTES_4096]->Pull();
    }

    else if (size <= static_cast<size_t>(MemorySize::BYTES_8192)) {
        ptr = mSLMemPoolsDict_Size[MemorySize::BYTES_8192]->Pull();
    }

    /* 메모리풀에 없다면 할당 */ 
    //if (ptr == nullptr) {
    //    ptr = ::_aligned_malloc(sizeof(SLIST_ENTRY) + size, MEMORY_ALLOCATION_ALIGNMENT);
    //    ptr = reinterpret_cast<void*>(reinterpret_cast<char*>(ptr) + sizeof(SLIST_ENTRY));
    //}

    return ptr;
}

void* MemoryManager::Allocate(std::string name)
{
   // while (mAtomicFlag.test_and_set(std::memory_order_acquire)); // Locking

    void* ptr = nullptr;

    // Find a suitable memory pool
    for (auto PoolIter : mSLMemPoolsDict_Name) {
        if (PoolIter.first == name) {
            ptr = PoolIter.second->Pull();
            break;
        }
    }

  //  mAtomicFlag.clear(std::memory_order_release); // Unlocking
    return ptr;
}

void MemoryManager::Free(std::string mpName, void* ptr)
{
    /* Locking */
   // while (mAtomicFlag.test_and_set(std::memory_order_acquire)) { };

    
    if (mSLMemPoolsDict_Name[mpName] != nullptr) {
        mSLMemPoolsDict_Name[mpName]->Push(ptr);
    }

    /* UnLocking */
   // mAtomicFlag.clear(std::memory_order_release);
}

void MemoryManager::Free(size_t size, void* ptr)
{
   // LOG_MGR->Cout("Free : ", ptr, "\n");

    ///* Locking */
    //while (mAtomicFlag.test_and_set(std::memory_order_acquire)){ };

    if (size <= static_cast<size_t>(MemorySize::BYTES_32)) {
        mSLMemPoolsDict_Size[MemorySize::BYTES_32]->Push(ptr);
    }
    else if (size <= static_cast<size_t>(MemorySize::BYTES_64)) {
        mSLMemPoolsDict_Size[MemorySize::BYTES_64]->Push(ptr);
    }
    else if (size <= static_cast<size_t>(MemorySize::BYTES_128)) {
        mSLMemPoolsDict_Size[MemorySize::BYTES_128]->Push(ptr);

    }
    else if (size <= static_cast<size_t>(MemorySize::BYTES_256)) {
        mSLMemPoolsDict_Size[MemorySize::BYTES_256]->Push(ptr);

    }
    else if (size <= static_cast<size_t>(MemorySize::BYTES_512)) {
        mSLMemPoolsDict_Size[MemorySize::BYTES_512]->Push(ptr);

    }
    else if (size <= static_cast<size_t>(MemorySize::BYTES_1024)) {
        mSLMemPoolsDict_Size[MemorySize::BYTES_1024]->Push(ptr);

    }
    else if (size <= static_cast<size_t>(MemorySize::BYTES_2048)) {
        mSLMemPoolsDict_Size[MemorySize::BYTES_2048]->Push(ptr);

    }
    else if (size <= static_cast<size_t>(MemorySize::BYTES_4096)) {
        mSLMemPoolsDict_Size[MemorySize::BYTES_4096]->Push(ptr);

    }
    else if (size <= static_cast<size_t>(MemorySize::BYTES_8192)) {
        mSLMemPoolsDict_Size[MemorySize::BYTES_8192]->Push(ptr);

    }
  
  /*  if (mSLMemPoolsDict_Size[static_cast<MemorySize>(size)] != nullptr) {
       mSLMemPoolsDict_Size[static_cast<MemorySize>(size)]->Push(ptr);
    }*/
  
    /* UnLocking */
    //mAtomicFlag.clear(std::memory_order_release);
}
