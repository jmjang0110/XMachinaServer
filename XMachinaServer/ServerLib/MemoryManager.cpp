#include "pch.h"
#include "MemoryManager.h"


DEFINE_SINGLETON(MemoryManager);
MemoryManager::MemoryManager()
{
}

MemoryManager::~MemoryManager()
{

}

bool MemoryManager::InitMemories()
{
    /* Memory�� �̸� �Ҵ��س���! */

    /* 1. �޸� ������� �����ϴ� �޸� Ǯ  */
    int blockNum = 50;
    AddSListMemoryPool(MemorySize::BYTES_32);
    for (int i = 0; i < blockNum; ++i) {
        mSLMemPoolsDict_Size[MemorySize::BYTES_32]->AddMemory();
    }

    AddSListMemoryPool(MemorySize::BYTES_64);
    for (int i = 0; i < blockNum; ++i) {
        mSLMemPoolsDict_Size[MemorySize::BYTES_64]->AddMemory();
    }


    AddSListMemoryPool(MemorySize::BYTES_128);
    for (int i = 0; i < blockNum; ++i) {
        mSLMemPoolsDict_Size[MemorySize::BYTES_128]->AddMemory();
    }


    AddSListMemoryPool(MemorySize::BYTES_256);
    for (int i = 0; i < blockNum; ++i) {
        mSLMemPoolsDict_Size[MemorySize::BYTES_256]->AddMemory();
    }


    AddSListMemoryPool(MemorySize::BYTES_512);
    for (int i = 0; i < blockNum; ++i) {
        mSLMemPoolsDict_Size[MemorySize::BYTES_512]->AddMemory();
    }


    /* 2. �޸� �̸����� �����ϴ� �޸� Ǯ */


    return true;
}

/* �޸�Ǯ�� �̸����� ���� */
void MemoryManager::AddSListMemoryPool(std::string mpName, size_t MemorySize)
{
    /* Locking */
    while (mAtomicFlag.test_and_set(std::memory_order_acquire))
    {
    };
    
    if (mSLMemPoolsDict_Name[mpName] == nullptr) {
        SListMemoryPool* Pool = new SListMemoryPool(MemorySize);
        /* dict (unordered_map) */
        mSLMemPoolsDict_Name[mpName] = Pool;
    }
    // �ش� �޸� ����� ���� Ǯ�� �̹� �����Ѵٸ� 
    else {
        mSLMemPoolsDict_Name[mpName]->AddMemory();
    }

    /* UnLocking */
    mAtomicFlag.clear(std::memory_order_release); 
}

/* �޸� Ǯ�� �޸� ������� ���� */
void MemoryManager::AddSListMemoryPool(MemorySize memSize)
{ 
    /* Locking */
    while (mAtomicFlag.test_and_set(std::memory_order_acquire))
    {
    };

    if (mSLMemPoolsDict_Size[memSize] == nullptr) {
        SListMemoryPool* Pool = new SListMemoryPool(static_cast<size_t>(memSize));
        /* dict (unordered_map) */
        mSLMemPoolsDict_Size[memSize] = Pool;
    }
    // �ش� �޸� ����� ���� Ǯ�� �̹� �����Ѵٸ� 
    else {
        mSLMemPoolsDict_Size[memSize]->AddMemory();
    }

    /* UnLocking */
    mAtomicFlag.clear(std::memory_order_release);

}

void* MemoryManager::Allocate(size_t size)
{
    while (mAtomicFlag.test_and_set(std::memory_order_acquire)); // Locking
   
    void* ptr = nullptr;

    // Find a suitable memory pool
    for (auto PoolIter : mSLMemPoolsDict_Size) {
        if (size <= PoolIter.second->GetMemorySize()) {
            ptr = PoolIter.second->Pull(); /* Pull Memory Pointer From SListMemoryPool */
            break;
        }
    }
    /* �޸�Ǯ�� ���ٸ� �Ҵ� */ 
    if (ptr == nullptr) {
        ptr = ::_aligned_malloc(sizeof(SLIST_ENTRY) + size, MEMORY_ALLOCATION_ALIGNMENT);
        ptr = reinterpret_cast<void*>(reinterpret_cast<char*>(ptr) + sizeof(SLIST_ENTRY));
    }

    mAtomicFlag.clear(std::memory_order_release); // Unlocking
    return ptr;
}

void* MemoryManager::Allocate(std::string name)
{
    while (mAtomicFlag.test_and_set(std::memory_order_acquire)); // Locking

    void* ptr = nullptr;

    // Find a suitable memory pool
    for (auto PoolIter : mSLMemPoolsDict_Name) {
        if (PoolIter.first == name) {
            ptr = PoolIter.second->Pull();
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

    if (mSLMemPoolsDict_Name[mpName] != nullptr) {
        mSLMemPoolsDict_Name[mpName]->Push(ptr);
    }

    /* UnLocking */
    mAtomicFlag.clear(std::memory_order_release);
}

void MemoryManager::Free(size_t size, void* ptr)
{
    ///* Locking */
    while (mAtomicFlag.test_and_set(std::memory_order_acquire))
    {
    };

    //if (mSLMemPoolsDict_Size[size] != nullptr) {
    //    mSLMemPoolsDict_Size[size]->Push(ptr);
    //}

    /* UnLocking */
    mAtomicFlag.clear(std::memory_order_release);
}
