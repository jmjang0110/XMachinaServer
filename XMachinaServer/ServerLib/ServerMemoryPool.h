#pragma once


/// ______________________________________
///             Memory Pool
/// ______________________________________
/// [ | | | | | | | | | | | | | | | | | | ] ( Memory Count )
/// [ ] => 32 memory Size 
///  �� Index 0
/// 
/// Available Index
/// [0][1][2][3][4][5][6]...[MemoryCount - 1]
/// 


class ServerMemoryPool
{
private:
    void* mBasePtr = nullptr;
    size_t mMemoryBlockSize = 0;
    size_t mMemoryBlockCnt = 0;

    Concurrency::concurrent_queue<size_t> mAvailableIndexes; // �ε��� ť

public:
    ServerMemoryPool();
    ~ServerMemoryPool();

public:
    void* Pull();
    void Push(void* ptr);
    void Init(size_t memoryBlockSize = 0, size_t memoryBlockCnt = 0);

};

