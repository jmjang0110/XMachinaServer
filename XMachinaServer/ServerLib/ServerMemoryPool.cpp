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
    ::_aligned_free(mBasePtr); // 메모리 해제
}

void* ServerMemoryPool::Pull()
{
    size_t index = 0;

    // concurrent_queue에서 사용 가능한 인덱스를 pop
    if (mAvailableIndexes.try_pop(index))
    {
        // 메모리 블록의 주소 계산
        return static_cast<char*>(mBasePtr) + (index * mMemoryBlockSize);
    }

    // 사용 가능한 블록이 없으면 nullptr 반환
    return nullptr;
}

void ServerMemoryPool::Push(void* ptr)
{
    // ptr을 char*로 변환하여 인덱스를 계산
    size_t index = static_cast<char*>(ptr) - static_cast<char*>(mBasePtr);
    index /= mMemoryBlockSize; // 메모리 블록 크기로 나누어 인덱스 추출

    assert(index < mMemoryBlockCnt); // 인덱스가 범위 내에 있는지 확인

    // 반환된 인덱스를 다시 큐에 넣음
    mAvailableIndexes.push(index);
}

void ServerMemoryPool::Init(size_t memoryBlockSize, size_t memoryBlockCnt)
{
    mMemoryBlockCnt = memoryBlockCnt;
    mMemoryBlockSize = memoryBlockSize;

    size_t allocSize = mMemoryBlockCnt * mMemoryBlockSize;
    mBasePtr = ::_aligned_malloc(allocSize, MEMORY_ALLOCATION_ALIGNMENT);

    // 모든 인덱스를 큐에 넣음 (사용 가능 상태로 초기화)
    for (size_t i = 0; i < mMemoryBlockCnt; ++i)
    {
        mAvailableIndexes.push(i);
    }
}