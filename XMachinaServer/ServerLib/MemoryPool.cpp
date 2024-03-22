#include "pch.h"
#include "MemoryPool.h"

MemoryPool::MemoryPool(size_t MemorySize, size_t numBlocks) 
    : mBlockSize(MemorySize), mMemorySize(MemorySize)
{
    // SLIST ��� �ʱ�ȭ
    ::InitializeSListHead(&mSListHeader);
    AddMemory();
}



MemoryPool::~MemoryPool()
{
    // �޸� ��ϵ��� ����
    PSLIST_ENTRY entry;
    while ((entry = ::InterlockedPopEntrySList(&mSListHeader)) != nullptr) {
        MemoryBlock* ptr = reinterpret_cast<MemoryBlock*>(entry);
        _aligned_free(ptr);
    }
}

void* MemoryPool::Allocate() 
{
    // SLIST���� �޸� ��� ��������
    PSLIST_ENTRY entry = ::InterlockedPopEntrySList(&mSListHeader);

    if (entry) {
        return reinterpret_cast<void*>(entry);
    }
    return nullptr; // ��� ������ ����� ���� ���
}

void MemoryPool::Free(void* ptr)
{
    // SLIST�� �޸� ��� �߰�
    ::InterlockedPushEntrySList(&mSListHeader, reinterpret_cast<PSLIST_ENTRY>(ptr));
}

void MemoryPool::AddMemory()
{
    // SLIST�� �޸� ��� �߰�
    MemoryBlock* ptr = static_cast<MemoryBlock*>(::_aligned_malloc(sizeof(MemoryBlock) + mMemorySize, MEMORY_ALLOCATION_ALIGNMENT));
    if (ptr) {
        InterlockedPushEntrySList(&mSListHeader, reinterpret_cast<PSLIST_ENTRY>(ptr));
    }

}
