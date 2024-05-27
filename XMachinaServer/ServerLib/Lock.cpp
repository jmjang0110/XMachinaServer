#include "pch.h"
#include "Lock.h"
#include "ThreadManager.h"



/// +-----------------------------
///          SPIN LOCK
/// -----------------------------+

Lock::SpinLock::SpinLock()
{

}

Lock::SpinLock::~SpinLock()
{
}

void Lock::SpinLock::Lock()
{
    while (mAtomicFlag.test_and_set(std::memory_order_acquire));
}

void Lock::SpinLock::UnLock()
{
    mAtomicFlag.clear(std::memory_order_release);
}


/// +-----------------------------
///            R/W LOCK 
/// -----------------------------+

//DEFINE_SINGLETON(Lock::RWLock);

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



