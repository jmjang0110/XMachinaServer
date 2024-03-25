#pragma once

namespace Lock
{
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

}

