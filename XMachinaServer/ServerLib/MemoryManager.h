#pragma once

/// +-----------------------------------------------
///					  MemoryManager 
/// ________________________________________________
///	����
///					
/// ----------------------------------------------+


#include "SListMemoryPool.h"
#include "ServerMemoryPool.h"


enum class MemorySize : UINT16
{
	BYTES_32   = 32,
	BYTES_64   = 64,
	BYTES_128  = 128,
	BYTES_256  = 256,
	BYTES_512  = 512, 
	BYTES_1024 = 1024,
	BYTES_2048 = 2048,
	BYTES_4096 = 4096,
	BYTES_8192 = 8192,
};

struct SListMemoryPoolInfo {
	std::string SListMemoryPoolName	= {};
	size_t		MemorySize		= {};
};

#define MEMORY MemoryManager::GetInst()
using Memory = class MemoryManager;
class MemoryManager
{
	DECLARE_SINGLETON(MemoryManager);

private:
	std::atomic_flag				mAtomicFlag       = ATOMIC_FLAG_INIT; // Atomic flag for thread safety
	std::unordered_map<std::string, ServerMemoryPool*> mMemoryPool_Name = {};
	std::unordered_map<MemorySize, ServerMemoryPool*>  mMemoryPool_Size = {};

public:
	MemoryManager();
	~MemoryManager();

public:
	bool InitMemories();

	void AddMemoryPool(MemorySize memSize, size_t memoryBlockCnt = 0);
	void AddMemoryPool(std::string memName, size_t memSize, size_t memoryBlockCnt = 0);


	void* Allocate(size_t size);
	void* Allocate(std::string name);
	
	void  Free(std::string mpName, void* ptr);  // �̸����� ������ �޸�Ǯ�� ptr �� �ִ´�. 
	void  Free(size_t size, void* ptr);			// ������� ������ �޸�Ǯ�� ptr �� �ִ´�. 

public:
	template<typename Type, typename... Args>
	static Type* New(Args&&... args);
	template<typename Type>
	static void Delete(Type* ptr);
	template<typename Type, typename... Args>
	static std::shared_ptr<Type> Make_Shared(Args&&... args);

};

template<typename Type, typename ...Args>
inline Type* MemoryManager::New(Args && ...args)
{
	Type* Memory = static_cast<Type*>(MEMORY->Allocate(sizeof(Type)));
	new(Memory)Type(std::forward<Args>(args)...); /* Placement New */
	return Memory;;
}

template<typename Type>
inline void MemoryManager::Delete(Type* ptr)
{
	ptr->~Type();
	MEMORY->Free(static_cast<size_t>(sizeof(Type)), ptr);
}

template<typename Type, typename ...Args>
inline std::shared_ptr<Type> MemoryManager::Make_Shared(Args&&... args)
{
	return std::shared_ptr<Type>{ New<Type>(std::forward<Args>(args)...), Delete<Type>};
}
