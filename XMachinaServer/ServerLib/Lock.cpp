#include "pch.h"
#include "Lock.h"
#include "ThreadManager.h"



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

void Lock::Lock::WriteLock(const char* name)
{
    // 동일한 쓰레드가 소유하고 있다면 무조건 성공.
    const UINT32 lockThreadId = (mLogFlag.load() & static_cast<UINT32>(LockFlag::Write_Thread_Mask)) >> 16;
    if (TLS_MGR->Get_TlsInfoData()->id == lockThreadId)
    {
        mWriteCount++;
        return;
    }

    // 아무도 소유 및 공유하고 있지 않을 때, 경합해서 소유권을 얻는다.
    const INT64 beginTick = ::GetTickCount64();
    const UINT32 desired = ((TLS_MGR->Get_TlsInfoData()->id << 16) & static_cast<UINT32>(LockFlag::Write_Thread_Mask));
    while (true)
    {
        for (UINT32 spinCount = 0; spinCount < static_cast<UINT32>(LockFlag::Max_Spin_Count); spinCount++)
        {
            UINT32 expected = static_cast<UINT32>(LockFlag::Empty_Mask);
            if (mLogFlag.compare_exchange_strong(OUT expected, desired))
            {
                mWriteCount++;
                return;
            }
        }

        if (::GetTickCount64() - beginTick >= static_cast<UINT32>(LockFlag::Acquire_Timeout_Tick))
            CRASH("LOCK_TIMEOUT");

        std::this_thread::yield(); // 다른 스레드에게 실행 시간을 양보한다. 
        // - 자기가 할일ㅇ의 우선순위가 낮을 경우
        // - 작업을 끝내고 다음 작업의 도착을 기다리는 경우 : 루프를 돌면서 작업의 도착을 확인하고 yield 해야한다. 
    }
}

void Lock::Lock::WriteUnlock(const char* name)
{
    // ReadLock 다 풀기 전에는 WriteUnlock 불가능.
    if ((mLogFlag.load() & static_cast<UINT32>(LockFlag::Read_Count_Mask)) != 0)
        CRASH("INVALID_UNLOCK_ORDER");

    const INT32 lockCount = --mWriteCount;
    if (lockCount == 0)
        mLogFlag.store(static_cast<UINT32>(LockFlag::Empty_Mask));
}

void Lock::Lock::ReadLock(const char* name)
{
    // 동일한 쓰레드가 소유하고 있다면 무조건 성공.
    const UINT32 lockThreadId = (mLogFlag.load() & static_cast<UINT32>(LockFlag::Write_Thread_Mask)) >> 16;
    if (TLS_MGR->Get_TlsInfoData()->id == lockThreadId)
    {
        mLogFlag.fetch_add(1);
        return;
    }

    // 아무도 소유하고 있지 않을 때 경합해서 공유 카운트를 올린다.
    const INT64 beginTick = ::GetTickCount64();
    while (true)
    {
        for (UINT32 spinCount = 0; spinCount < static_cast<UINT32>(LockFlag::Max_Spin_Count); spinCount++)
        {
            UINT32 expected = (mLogFlag.load() & static_cast<UINT32>(LockFlag::Read_Count_Mask));
            if (mLogFlag.compare_exchange_strong(OUT expected, expected + 1))
                return;
        }

        if (::GetTickCount64() - beginTick >= static_cast<UINT32>(LockFlag::Acquire_Timeout_Tick))
            CRASH("LOCK_TIMEOUT");

        std::this_thread::yield();
    }
}

void Lock::Lock::ReadUnlock(const char* name)
{
    if ((mLogFlag.fetch_sub(1) & static_cast<UINT32>(LockFlag::Read_Count_Mask)) == 0)
        CRASH("MULTIPLE_UNLOCK");
}
