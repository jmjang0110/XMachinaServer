#pragma once

/// +-----------------------------------------------
///					  MemoryManager 
/// ________________________________________________
///	����
///					
/// ----------------------------------------------+


#include "MemoryPool.h"

#define MEMORY MemoryManager::GetInst()
class MemoryManager
{
	DECLARE_SINGLETON(MemoryManager);

private:
	std::vector<MemoryPool*>	mMemoryPools = {};
	std::atomic_flag			mAtomicFlag  = ATOMIC_FLAG_INIT; // Atomic flag for thread safety

	std::unordered_map<size_t, MemoryPool*> mMemoryPoolsDict = {};
public:
	MemoryManager();
	~MemoryManager();

public:
	void  AddMemoryPool(size_t memorySize, size_t numMemoryBlocks);
	void* Allocate(size_t size);
	void  Free(size_t memorySize, void* ptr); // �޸�Ǯ���� ����ߴ� �����Ͷ�� �޸� ����� �˾ƾ��Ѵ�. 

};

namespace PoolAllocate 
{

}

