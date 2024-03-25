#pragma once

/// +-----------------------------------------------
///					  MemoryManager 
/// ________________________________________________
///	����
///					
/// ----------------------------------------------+


#include "SListMemoryPool.h"

namespace MemoryAllocator
{
	//template<typename Type, typename... Args>
	//Type* Malloc(Args&&... args) {
	//	Type* Memory = static_cast<Type*>();
	//	/* Placement New */
	//	new(Memory)Type(std::forward<Args>(args...));
	//	return Memory;;
	//}

}

struct SListMemoryPoolInfo {
	std::string SListMemoryPoolName	= {};
	size_t		MemorySize		= {};
};
#define MEMORY MemoryManager::GetInst()
class MemoryManager
{
	DECLARE_SINGLETON(MemoryManager);

private:
	std::vector<SListMemoryPool*>	mSListMemoryPools = {};
	std::atomic_flag				mAtomicFlag       = ATOMIC_FLAG_INIT; // Atomic flag for thread safety

	std::unordered_map<std::string, SListMemoryPool*> mSListMemoryPoolsDict = {};
public:
	MemoryManager();
	~MemoryManager();

public:
	void  AddSListMemoryPool(std::string mpName, size_t MemorySize, size_t numMemoryBlocks);
	void* Allocate(size_t size);
	void  Free(std::string mpName, void* ptr); // �޸�Ǯ���� ����ߴ� �����Ͷ�� �޸� ����� �˾ƾ��Ѵ�. 

};

