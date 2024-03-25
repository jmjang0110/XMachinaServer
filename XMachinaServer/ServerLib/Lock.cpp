#include "pch.h"
#include "Lock.h"

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
        SpinWait();   // ª�� ��� �ð� ���� �����մϴ�.
    }
}

void Lock::SpinLock::UnLock()
{
    mAtomicFlag.store(false, std::memory_order_release);
}

void Lock::SpinLock::SpinWait()
{
    // ���� ��� �ð��� ������ �� �ֽ��ϴ�.
    for (INT32 i = 0; i < mMaxSpinCount; ++i) {
        // �ƹ� �۾��� ���� �ʰ� �ð��� �Ҹ��մϴ�.
        volatile INT32 dummy = 0; // ª�� ��� �ð��� ���� volatile ���� ���
        
        for (volatile INT32 j = 0; j < 100; ++j) {
            dummy += j;
        }
        // ���� ���� ����
        if (FALSE == mAtomicFlag.load(std::memory_order_relaxed)) {
            return;
        }
        std::this_thread::yield(); // ª�� ��� �ð��� ���� yield
    }

}

