#pragma once


/// +-----------------------------------------------
///					  MemoryPool 
/// ________________________________________________
///				       메모리 풀  
/// 
/// Thread Safe 메모리 풀 
/// ----------------------------------------------+


DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT)
struct MemoryBlock 
{
    SLIST_ENTRY entry = {}; // SLIST 엔트리
};

DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT)
class MemoryPool {
private:
    SLIST_HEADER                mSListHeader  = {};       

    size_t                      mMemorySize   = {};
    size_t                      mBlockSize    = {};      // 할당할 메모리 블록의 크기


public:
    MemoryPool(size_t MemorySize, size_t numBlocks);
    ~MemoryPool();

public:
    void*   Allocate();
    void    Free(void* ptr);
    void    AddMemory();


    size_t GetBlockSize() const { return mBlockSize; }

};