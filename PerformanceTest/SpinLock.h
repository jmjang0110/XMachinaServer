#pragma once


// 스핀락 클래스 포함
class SpinLock {

private:
    std::atomic_flag flag;

public:
    SpinLock() {
        flag.test_and_set(ATOMIC_FLAG_INIT) ;
    }

    void lock() {
        while (flag.test_and_set(std::memory_order_acquire)) {
            // 바쁜 대기 (busy-wait)
        }
    }

    void unlock() {
        flag.clear(std::memory_order_release);
    }


};


/// +---------------------------------
///  SRWLock - Slim Read Write Lock
/// ---------------------------------+
class SRWLock
{
    typedef enum { NOT_INIT, UNLOCKED, READLOCKED, WRITELOCKED } MASK;
private:
    SRWLOCK mSrwLock; // MSDN 
    MASK lockMask = NOT_INIT;

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
