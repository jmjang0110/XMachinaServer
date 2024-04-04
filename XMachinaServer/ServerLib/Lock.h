#pragma once

namespace Lock
{
    /* SpinLock */
    class SpinLock {
    private:
        std::atomic<bool> mAtomicFlag   = {};
        UINT64            mMaxSpinCount = 5000;
    public:
        SpinLock();
        ~SpinLock();
        void Lock();
        void UnLock();
        void SpinWait();
    };

    /* R/W Lock */
    class RWLock {
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

}

