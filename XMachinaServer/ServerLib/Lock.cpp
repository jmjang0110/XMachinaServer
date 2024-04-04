#include "pch.h"
#include "Lock.h"

/// +-----------------------------
///          SPIN LOCK
/// -----------------------------+

Lock::SpinLock::SpinLock()
{
    mAtomicFlag.store(false);

}

Lock::SpinLock::~SpinLock()
{
}

void Lock::SpinLock::Lock()
{
    while (mAtomicFlag.exchange(true, std::memory_order_acquire)) {
        SpinWait();   // 짧은 대기 시간 동안 스핀합니다.
    }
}

void Lock::SpinLock::UnLock()
{
    mAtomicFlag.store(false, std::memory_order_release);
}

void Lock::SpinLock::SpinWait()
{
    // 스핀 대기 시간을 조절할 수 있습니다.
    for (INT32 i = 0; i < mMaxSpinCount; ++i) {
        // 아무 작업도 하지 않고 시간을 소모합니다.
        volatile INT32 dummy = 0; // 짧은 대기 시간을 위해 volatile 변수 사용
        
        for (volatile INT32 j = 0; j < 100; ++j) {
            dummy += j;
        }
        // 루프 종료 조건
        if (FALSE == mAtomicFlag.load(std::memory_order_relaxed)) {
            return;
        }
        std::this_thread::yield(); // 짧은 대기 시간을 위해 yield
    }

}


/// +-----------------------------
///            R/W LOCK 
/// -----------------------------+

Lock::RWLock::RWLock()
    : mReaderCount(0), mWriterCount(0) {}

Lock::RWLock::~RWLock()
{
}
void Lock::RWLock::lockRead()
{
    while (true) {
        while (mWriterCount.load() > 0) {
            std::this_thread::yield(); // 기다림
        }
        mReaderCount.fetch_add(1, std::memory_order_acquire);
        if (mWriterCount.load() == 0) {
            break;
        }
        mReaderCount.fetch_sub(1, std::memory_order_release);
    }
}

void Lock::RWLock::unlockRead()
{
    mReaderCount.fetch_sub(1, std::memory_order_release);
}

void Lock::RWLock::lockWrite()
{
    mWriterCount.fetch_add(1, std::memory_order_acquire);
    while (mReaderCount.load() > 0) {
        std::this_thread::yield(); // 기다림
    }
}

void Lock::RWLock::unlockWrite()
{
    mWriterCount.fetch_sub(1, std::memory_order_release);
}
