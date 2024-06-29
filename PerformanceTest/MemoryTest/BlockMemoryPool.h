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
		// �̸� ������ ũ�⸸ŭ �޸� ����� �Ҵ��մϴ�.
		expandPool(pool_Size);
	}

	~BlockMemoryPool_Vector() {
		// �Ҵ�� ��� �޸𸮸� �����մϴ�.
		for (auto ptr : pool) {
			::operator delete(ptr);
		}
	}

	T* allocate() {
		SL.LockWrite();

		if (pool.empty()) {
			// Ǯ�� ����� �� �ִ� �޸𸮰� ������ Ǯ�� Ȯ���մϴ�.
			expandPool(defaultPoolSize);
		}
		// Ǯ���� �޸� ����� �ϳ� ������ ��ȯ�մϴ�.
		T* ptr = static_cast<T*>(pool.back());
		pool.pop_back();

		SL.UnlockWrite();

		return ptr;
	}

	void deallocate(T* ptr) {
		SL.LockWrite();

		// ����� �޸� ����� �ٽ� Ǯ�� �ֽ��ϴ�.
		pool.push_back(ptr);
		SL.UnlockWrite();

	}
};

template<typename T>
class BlockMemoryPool_Array
{
private:
    T** pool; // ������ �迭�� �޸� ����� �����մϴ�
    size_t poolSize; // ���� Ǯ�� ũ��
    size_t capacity; // Ǯ�� �ִ� ũ��
    size_t top; // Ǯ�� ����� �ε���
    SRWLOCK mSL;
    const size_t defaultPoolSize = 10; // �⺻ Ǯ ũ��

    void expandPool(size_t size) {
        // Ǯ�� ũ�⸦ �ø��� ���� �� �迭�� �Ҵ��մϴ�
        size_t newCapacity = capacity + size;
        T** newPool = new T * [newCapacity];

        // ���� Ǯ�� ������ �� Ǯ�� �����մϴ�
        for (size_t i = 0; i < poolSize; ++i) {
            newPool[i] = pool[i];
        }

        // �� ����� �߰��մϴ�
        for (size_t i = poolSize; i < newCapacity; ++i) {
            newPool[i] = static_cast<T*>(::operator new(sizeof(T)));
        }

        // ���� Ǯ�� �����մϴ�
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