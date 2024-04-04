#include "pch.h"
#include "SListMemoryPool.h"
/*
Concurrency_Queue 
*/

SListMemoryPool::SListMemoryPool(size_t MemorySize) 
    : mMemorySize(MemorySize)
{
    mNumBlocks.store(0);
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
    PSLIST_ENTRY ptr = ::InterlockedPopEntrySList(&mSListHeader);
  

    if (ptr) {
        /* ptr-------->ptr                  */       
        /* ��            ��                   */       
        /* [SLIST_ENTRY][ ... memory ... ]  */
        return reinterpret_cast<void*>(reinterpret_cast<char*>(ptr) + sizeof(SLIST_ENTRY));
    }
    return nullptr; // ��� ������ ����� ���� ���
}

void SListMemoryPool::Push(void* ptr)
{
    mNumBlocks.fetch_add(1);

    // SLIST�� �޸� ��� �߰�
        /* ptr<--------ptr                  */       
        /* ��            ��                   */       
        /* [SLIST_ENTRY][ ... memory ... ]  */
    ::InterlockedPushEntrySList(&mSListHeader, reinterpret_cast<PSLIST_ENTRY>(reinterpret_cast<char*>(ptr) - sizeof(SLIST_ENTRY)));
}

void SListMemoryPool::AddMemory()
{
    // SLIST�� �޸� ��� �߰�
    /* [SLIST_ENTRY][ ... memory ... ] */
    SListMemoryBlock* ptr = static_cast<SListMemoryBlock*>(::_aligned_malloc(sizeof(SLIST_ENTRY) + mMemorySize, MEMORY_ALLOCATION_ALIGNMENT));
    if (ptr) {
        mNumBlocks.fetch_add(1);
        InterlockedPushEntrySList(&mSListHeader, reinterpret_cast<PSLIST_ENTRY>(ptr));
    }

}
