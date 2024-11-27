#pragma once


/// ______________________________________
///             Memory Pool
/// ______________________________________
/// [ | | | | | | | | | | | | | | | | | | ] ( Memory Count )
/// [ ] => 32 , 64, 128, 256, 512 ,,, Memory Size 
///  °Ë Index 0
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

    Concurrency::concurrent_queue<size_t> mAvailableIndexes; // ¿Œµ¶Ω∫ ≈•

public:
    ServerMemoryPool();
    ~ServerMemoryPool();

public:
    void* Pull();
    bool Push(void* ptr);
    void Init(size_t memoryBlockSize = 0, size_t memoryBlockCnt = 0);

};

