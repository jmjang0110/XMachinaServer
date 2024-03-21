#pragma once


/// +-----------------------------------------------
///					  MemoryPool 
/// ________________________________________________
///				       �޸� Ǯ  
/// 
/// Thread Safe �޸� Ǯ 
/// ----------------------------------------------+


DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT)
struct MemoryBlock 
{
    SLIST_ENTRY entry = {}; // SLIST ��Ʈ��
};

DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT)
class MemoryPool {
private:
    SLIST_HEADER                mSListHeader  = {};       
    size_t                      mBlockSize    = {};      // �Ҵ��� �޸� ����� ũ��
    std::vector<MemoryBlock*>   mMemoryBlocks = {};      // �޸� ��� ���� 

public:
    MemoryPool(size_t MemorySize, size_t numBlocks);
    ~MemoryPool();

public:
    void*   Allocate();
    void    Free(void* ptr);
};