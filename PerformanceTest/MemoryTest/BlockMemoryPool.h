#pragma once
#include "SpinLock.h"

template<typename T>
class BlockMemoryPool_Vector
{
private:
	std::vector<void*> pool;
	const size_t defaultPoolSize = 10;
	SRWLock SL;

	void expandPool(size_t size) {
		for (size_t i = 0; i < size; ++i) {
			pool.push_back(::operator new(sizeof(T)));
		}
	}

public:
	BlockMemoryPool_Vector() {};
	BlockMemoryPool_Vector(size_t pool_Size) {
		// 미리 지정한 크기만큼 메모리 블록을 할당합니다.
		expandPool(pool_Size);
	}

	~BlockMemoryPool_Vector() {
		// 할당된 모든 메모리를 해제합니다.
		for (auto ptr : pool) {
			::operator delete(ptr);
		}
	}

	T* allocate() {
		SL.LockWrite();

		if (pool.empty()) {
			// 풀에 사용할 수 있는 메모리가 없으면 풀을 확장합니다.
			expandPool(defaultPoolSize);
		}
		// 풀에서 메모리 블록을 하나 꺼내서 반환합니다.
		T* ptr = static_cast<T*>(pool.back());
		pool.pop_back();

		SL.UnlockWrite();

		return ptr;
	}

	void deallocate(T* ptr) {
		SL.LockWrite();

		// 사용한 메모리 블록을 다시 풀에 넣습니다.
		pool.push_back(ptr);
		SL.UnlockWrite();

	}
};

template<typename T>
class BlockMemoryPool_Array
{
private:
    T** pool; // 포인터 배열로 메모리 블록을 관리합니다
    size_t poolSize; // 현재 풀의 크기
    size_t capacity; // 풀의 최대 크기
    size_t top; // 풀의 꼭대기 인덱스
    SRWLOCK mSL;
    const size_t defaultPoolSize = 10; // 기본 풀 크기

    void expandPool(size_t size) {
        // 풀의 크기를 늘리기 위해 새 배열을 할당합니다
        size_t newCapacity = capacity + size;
        T** newPool = new T * [newCapacity];

        // 기존 풀의 내용을 새 풀로 복사합니다
        for (size_t i = 0; i < poolSize; ++i) {
            newPool[i] = pool[i];
        }

        // 새 블록을 추가합니다
        for (size_t i = poolSize; i < newCapacity; ++i) {
            newPool[i] = static_cast<T*>(::operator new(sizeof(T)));
        }

        // 기존 풀을 해제합니다
        delete[] pool;
        pool = newPool;
        capacity = newCapacity;
        poolSize = newCapacity;
        top = poolSize - 1;
    }

public:
    BlockMemoryPool_Array(size_t pool_Size = 10)
        : poolSize(pool_Size), capacity(pool_Size), top(pool_Size - 1) 
    {
        InitializeSRWLock(&mSL);

        pool = new T * [pool_Size];
        for (size_t i = 0; i < pool_Size; ++i) {
            pool[i] = static_cast<T*>(::operator new(sizeof(T)));
        }
    }

    ~BlockMemoryPool_Array() {
        for (size_t i = 0; i < poolSize; ++i) {
            ::operator delete(pool[i]);
        }
        delete[] pool;
    }

    T* allocate() {
        AcquireSRWLockExclusive(&mSL);

        if (top == static_cast<size_t>(-1)) {
            expandPool(defaultPoolSize);
        }
        T* ptr = pool[top--];
        ReleaseSRWLockExclusive(&mSL);
        return ptr;
    }

    void deallocate(T* ptr) {
        AcquireSRWLockExclusive(&mSL);

        pool[++top] = ptr;
        ReleaseSRWLockExclusive(&mSL);
    }
};

template<typename T>
class MemoryPool_MyArr {
private:
    static const int MAX_BLOCKS = 65535;
    static const int BLOCK_SIZE = sizeof(T);

    BYTE* mPool;
    bool* mAllocated;
    SRWLOCK mSL;
public:
    MemoryPool_MyArr() {
        InitializeSRWLock(&mSL);
        mPool = new BYTE[MAX_BLOCKS * BLOCK_SIZE];
        mAllocated = new bool[MAX_BLOCKS]();
    }

    ~MemoryPool_MyArr() {
        delete[] mPool;
        delete[] mAllocated;
    }

    T* Allocate() {
        AcquireSRWLockExclusive(&mSL);

        for (int i = 0; i < MAX_BLOCKS; ++i) {
            if (!mAllocated[i]) {
                mAllocated[i] = true;
                ReleaseSRWLockExclusive(&mSL);
                return reinterpret_cast<T*>(mPool + i * BLOCK_SIZE);
            }
        }

        ReleaseSRWLockExclusive(&mSL);
        throw std::bad_alloc();
    }

    void Deallocate(void* memory) {
        AcquireSRWLockExclusive(&mSL);

        int index = (reinterpret_cast<BYTE*>(memory) - mPool) / BLOCK_SIZE;
        if (index >= 0 && index < MAX_BLOCKS) {
            mAllocated[index] = false;
        }

        ReleaseSRWLockExclusive(&mSL);
    }
};


class VariableMemoryPool {

private:
    SRWLOCK mSL;
    BYTE* mPool;

public:



};