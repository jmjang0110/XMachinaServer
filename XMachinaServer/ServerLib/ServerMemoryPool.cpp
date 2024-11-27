#include "pch.h"
#include "ServerMemoryPool.h"

#include "pch.h"
#include "ServerMemoryPool.h"
#include <concurrent_queue.h>
#include <cassert>

ServerMemoryPool::ServerMemoryPool()
{
    mBasePtr = nullptr;
}

ServerMemoryPool::~ServerMemoryPool()
{
    ::_aligned_free(mBasePtr); // �޸� ����
}

void* ServerMemoryPool::Pull()
{
    size_t index = 0;

    // concurrent_queue���� ��� ������ �ε����� pop
    if (mAvailableIndexes.try_pop(index))
    {
        // �޸� ����� �ּ� ���
        return static_cast<char*>(mBasePtr) + (index * mMemoryBlockSize);
    }

    // ��� ������ ����� ������ nullptr ��ȯ
    return ::operator new(mMemoryBlockSize);
}

bool ServerMemoryPool::Push(void* ptr)
{
    // ptr�� char*�� ��ȯ�Ͽ� �ε����� ���
    size_t index = static_cast<char*>(ptr) - static_cast<char*>(mBasePtr);
    index /= mMemoryBlockSize; // �޸� ��� ũ��� ������ �ε��� ����

    if (index < mMemoryBlockCnt || index >= mMemoryBlockCnt)
        return false;

    assert(index < mMemoryBlockCnt || index >= mMemoryBlockCnt); // �ε����� ���� ���� �ִ��� Ȯ��

    // ��ȯ�� �ε����� �ٽ� ť�� ����
    mAvailableIndexes.push(index);
}

void ServerMemoryPool::Init(size_t memoryBlockSize, size_t memoryBlockCnt)
{
    mMemoryBlockCnt = memoryBlockCnt;
    mMemoryBlockSize = memoryBlockSize;

    size_t allocSize = mMemoryBlockCnt * mMemoryBlockSize;
    mBasePtr = ::_aligned_malloc(allocSize, MEMORY_ALLOCATION_ALIGNMENT);

    // ��� �ε����� ť�� ���� (��� ���� ���·� �ʱ�ȭ)
    for (size_t i = 0; i < mMemoryBlockCnt; ++i)
    {
        mAvailableIndexes.push(i);
    }
}