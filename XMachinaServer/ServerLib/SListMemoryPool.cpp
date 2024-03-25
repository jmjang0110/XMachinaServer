#include "pch.h"
#include "SListMemoryPool.h"
/*
Concurrency_Queue 
*/

SListMemoryPool::SListMemoryPool(size_t MemorySize, size_t numBlocks) 
    : mMemorySize(MemorySize)
{
    mNumBlocks.store(numBlocks);

    // SLIST ��� �ʱ�ȭ
    ::InitializeSListHead(&mSListHeader);
    AddMemory();
}



SListMemoryPool::~SListMemoryPool()
{
    // �޸� ��ϵ��� ����
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

    // SLIST���� �޸� ��� ��������
    PSLIST_ENTRY entry = ::InterlockedPopEntrySList(&mSListHeader);
  

    if (entry) {
        return reinterpret_cast<void*>(entry);
    }
    return nullptr; // ��� ������ ����� ���� ���
}

void SListMemoryPool::Push(void* ptr)
{
    mNumBlocks.fetch_add(1);

    // SLIST�� �޸� ��� �߰�
    ::InterlockedPushEntrySList(&mSListHeader, reinterpret_cast<PSLIST_ENTRY>(ptr));
}

void SListMemoryPool::AddMemory()
{

    // SLIST�� �޸� ��� �߰�
    SListMemoryBlock* ptr = static_cast<SListMemoryBlock*>(::_aligned_malloc(sizeof(SListMemoryBlock) + mMemorySize, MEMORY_ALLOCATION_ALIGNMENT));
    if (ptr) {
        mNumBlocks.fetch_add(1);
        InterlockedPushEntrySList(&mSListHeader, reinterpret_cast<PSLIST_ENTRY>(ptr));
    }

}
