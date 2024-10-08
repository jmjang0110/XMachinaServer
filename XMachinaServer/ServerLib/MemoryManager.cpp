#include "pch.h"
#include "MemoryManager.h"


DEFINE_SINGLETON(MemoryManager);
MemoryManager::MemoryManager()
{
}

MemoryManager::~MemoryManager()
{
    /* Delete SListMemoryPool !!! */
    for (auto& entry : mMemoryPool_Name) {
        delete entry.second;
        mMemoryPool_Name[entry.first] = nullptr;

    }
    mMemoryPool_Name.clear();

    for (auto& entry : mMemoryPool_Size) {
        ServerMemoryPool* ptr = entry.second;
        delete ptr;
        mMemoryPool_Size[entry.first] = nullptr;
    }
    mMemoryPool_Size.clear();

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
    if (mMemoryPool_Size.find(memSize) != mMemoryPool_Size.end())
        return;

    mMemoryPool_Size[memSize] = new ServerMemoryPool();
    mMemoryPool_Size[memSize]->Init(static_cast<size_t>(memSize), memoryBlockCnt);

}

void MemoryManager::AddMemoryPool(std::string memName, size_t memSize, size_t memoryBlockCnt)
{
    if (mMemoryPool_Name.find(memName) != mMemoryPool_Name.end())
        return;

    mMemoryPool_Name[memName] = new ServerMemoryPool();
    mMemoryPool_Name[memName]->Init(static_cast<size_t>(memSize), memoryBlockCnt);
}


void* MemoryManager::Allocate(size_t size)
{
    void* ptr = nullptr;

    if (size <= static_cast<size_t>(MemorySize::BYTES_32)) {
        ptr = mMemoryPool_Size[MemorySize::BYTES_32]->Pull();
    }
    else if (size <= static_cast<size_t>(MemorySize::BYTES_64)) {
        ptr = mMemoryPool_Size[MemorySize::BYTES_64]->Pull();

    }
    else if (size <= static_cast<size_t>(MemorySize::BYTES_128)) {
        ptr = mMemoryPool_Size[MemorySize::BYTES_128]->Pull();

    }
    else if (size <= static_cast<size_t>(MemorySize::BYTES_256)) {
        ptr = mMemoryPool_Size[MemorySize::BYTES_256]->Pull();

    }
    else if (size <= static_cast<size_t>(MemorySize::BYTES_512)) {
        ptr = mMemoryPool_Size[MemorySize::BYTES_512]->Pull();

    }
    else if (size <= static_cast<size_t>(MemorySize::BYTES_1024)) {
        ptr = mMemoryPool_Size[MemorySize::BYTES_1024]->Pull();

    }
    else if (size <= static_cast<size_t>(MemorySize::BYTES_2048)) {
        ptr = mMemoryPool_Size[MemorySize::BYTES_2048]->Pull();
    }

    else if (size <= static_cast<size_t>(MemorySize::BYTES_4096)) {
        ptr = mMemoryPool_Size[MemorySize::BYTES_4096]->Pull();
    }

    else if (size <= static_cast<size_t>(MemorySize::BYTES_8192)) {
        ptr = mMemoryPool_Size[MemorySize::BYTES_8192]->Pull();
    }

    return ptr;
}

void* MemoryManager::Allocate(std::string name)
{
    void* ptr = nullptr;
    for (auto PoolIter : mMemoryPool_Name) {
        if (PoolIter.first == name) {
            ptr = PoolIter.second->Pull();
            break;
        }
    }
    return ptr;
}

void MemoryManager::Free(std::string mpName, void* ptr)
{
    if (mMemoryPool_Name[mpName] != nullptr) {
        mMemoryPool_Name[mpName]->Push(ptr);
    }
}

void MemoryManager::Free(size_t size, void* ptr)
{
    if (size <= static_cast<size_t>(MemorySize::BYTES_32)) {
        mMemoryPool_Size[MemorySize::BYTES_32]->Push(ptr);
    }
    else if (size <= static_cast<size_t>(MemorySize::BYTES_64)) {
        mMemoryPool_Size[MemorySize::BYTES_64]->Push(ptr);
    }
    else if (size <= static_cast<size_t>(MemorySize::BYTES_128)) {
        mMemoryPool_Size[MemorySize::BYTES_128]->Push(ptr);

    }
    else if (size <= static_cast<size_t>(MemorySize::BYTES_256)) {
        mMemoryPool_Size[MemorySize::BYTES_256]->Push(ptr);

    }
    else if (size <= static_cast<size_t>(MemorySize::BYTES_512)) {
        mMemoryPool_Size[MemorySize::BYTES_512]->Push(ptr);

    }
    else if (size <= static_cast<size_t>(MemorySize::BYTES_1024)) {
        mMemoryPool_Size[MemorySize::BYTES_1024]->Push(ptr);

    }
    else if (size <= static_cast<size_t>(MemorySize::BYTES_2048)) {
        mMemoryPool_Size[MemorySize::BYTES_2048]->Push(ptr);

    }
    else if (size <= static_cast<size_t>(MemorySize::BYTES_4096)) {
        mMemoryPool_Size[MemorySize::BYTES_4096]->Push(ptr);

    }
    else if (size <= static_cast<size_t>(MemorySize::BYTES_8192)) {
        mMemoryPool_Size[MemorySize::BYTES_8192]->Push(ptr);

    }
}
