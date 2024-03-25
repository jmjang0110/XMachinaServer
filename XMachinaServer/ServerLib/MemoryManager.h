#pragma once

/// +-----------------------------------------------
///					  MemoryManager 
/// ________________________________________________
///	구조
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
	void  Free(std::string mpName, void* ptr); // 메모리풀에서 사용했던 포인터라면 메모리 사이즈를 알아야한다. 

};

