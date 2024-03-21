#include "pch.h"
#include "MemoryPool.h"

MemoryPool::MemoryPool(size_t MemorySize, size_t numBlocks) 
    : mBlockSize(MemorySize)
{
    // SLIST ��� �ʱ�ȭ
    InitializeSListHead(&mSListHeader);

    // �޸� ����� �̸� �Ҵ��Ͽ� SLIST�� �߰�
    for (size_t i = 0; i < numBlocks; ++i) {
        MemoryBlock* ptr = static_cast<MemoryBlock*>(_aligned_malloc(sizeof(MemoryBlock) + MemorySize, MEMORY_ALLOCATION_ALIGNMENT));
        
        if (ptr == nullptr) {
            // �Ҵ� ���� ��, ������ �Ҵ��� �޸� ����
            for (size_t j = 0; j < i; ++j) {
                _aligned_free(mMemoryBlocks[j]);
            }
        }

        InterlockedPushEntrySList(&mSListHeader, reinterpret_cast<PSLIST_ENTRY>(ptr));
        mMemoryBlocks.push_back(ptr); // �޸� ����� �����ϱ� ���� ���Ϳ� �߰�
    }
}


MemoryPool::~MemoryPool()
{
    // �Ҵ�� �޸� ��ϵ� ����
    for (auto ptr : mMemoryBlocks) {
        _aligned_free(ptr);
    }
}

void* MemoryPool::Allocate() 
{
    // SLIST���� �޸� ��� ��������
    PSLIST_ENTRY entry = InterlockedPopEntrySList(&mSListHeader);

    if (entry) {
        return reinterpret_cast<void*>(entry);
    }
    return nullptr; // ��� ������ ����� ���� ���
}

void MemoryPool::Free(void* ptr)
{
    // SLIST�� �޸� ��� �߰�
    InterlockedPushEntrySList(&mSListHeader, reinterpret_cast<PSLIST_ENTRY>(ptr));
}
