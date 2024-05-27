#pragma once

namespace Lock
{
    /* SpinLock */
    class SpinLock {
    private:
        std::atomic_flag  mAtomicFlag = ATOMIC_FLAG_INIT;
    public:
        SpinLock();
        ~SpinLock();
        void Lock();
        void UnLock();
    };

//#define RWLOCK RWLock::GetInst()
    class RWLock {
    private:
       // DECLARE_SINGLETON(RWLock);

    private:
        std::atomic<int> mReaderCount;
        std::atomic<int> mWriterCount;

    public:
        RWLock();
        ~RWLock();
        void lockRead();
        void unlockRead();
        void lockWrite();
        void unlockWrite();
        
    };

    /// +---------------------------------
    ///  SRWLock - Slim Read Write Lock
    /// ---------------------------------+
    class SRWLock
    {
        typedef enum { NOT_INIT, UNLOCKED, READLOCKED, WRITELOCKED} MASK;
    private:
        SRWLOCK mSrwLock; // MSDN 
        MASK lockMask            = NOT_INIT;

    public:
        SRWLock() { InitializeSRWLock(&mSrwLock); }

        /* Read Lock - Unlock */
        void LockRead() { 
            AcquireSRWLockShared(&mSrwLock); 
        }
        void UnlockRead() { 
            ReleaseSRWLockShared(&mSrwLock);
        }

        /* Write Lock - Unlock */
        void LockWrite() { 
            AcquireSRWLockExclusive(&mSrwLock); 
        }
        void UnlockWrite() {
            ReleaseSRWLockExclusive(&mSrwLock);
        }
        
        // 읽기 잠금을 획득한 상태인지 확인
        bool TryReadLocked() const { 
            return TryAcquireSRWLockShared(const_cast<SRWLOCK*>(&mSrwLock)) == TRUE; 
        }
        bool TryWriteLocked() const {
            return TryAcquireSRWLockExclusive(const_cast<SRWLOCK*>(&mSrwLock)) == TRUE; 
        }

    };

    class SRW_WriteLockGaurd
    {
    private:
        SRWLock& _lock;
    public:
        SRW_WriteLockGaurd(SRWLock& lock) : _lock(lock) { _lock.LockWrite(); }
        ~SRW_WriteLockGaurd() { _lock.UnlockWrite(); };
        
    };
#define WriteLockScope(srwLock) SRW_WriteLockGuard(srwLock)

}

