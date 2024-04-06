#include "pch.h"
#include "SListMemoryPool.h"
#include "ThreadManager.h"

/*
Concurrency_Queue 
*/

SListMemoryPool::SListMemoryPool(size_t MemorySize) 
    : mMemorySize(MemorySize)
{
    mNumBlocks.store(0);
    // SLIST ��� �ʱ�ȭ
    ::InitializeSListHead(&mSListHeader);
     AddMemory();
}



SListMemoryPool::~SListMemoryPool()
{
    // �޸� ��ϵ��� ����
    PSLIST_ENTRY entry;
    while ((entry = ::InterlockedPopEntrySList(&mSListHeader)) != nullptr) {
        SListMemoryBlock* ptr = reinterpret_cast<SListMemoryBlock*>(entry);
        _aligned_free(ptr);
    }
    mNumBlocks.store(0);

    mPushCount.store(0);
    mPullCount.store(0);

}

void* SListMemoryPool::Pull() 
{
    //if (mNumBlocks.load() == 0)
     //   return nullptr;



    // SLIST���� �޸� ��� ��������
    mNumBlocks.fetch_sub(1);
    PSLIST_ENTRY ptr = ::InterlockedPopEntrySList(&mSListHeader);

    if (TLS_MGR->Get_TlsInfoData()) {
       
       //std::cout  << TLS_MGR->Get_TlsSendBufFactory()->SendBufFactory << " �߱� ���̵� : " << TLS_MGR->Get_TlsInfoData()->id << " �޸� ������ - " << reinterpret_cast<void*>(reinterpret_cast<char*>(ptr) + sizeof(SLIST_ENTRY)) << " "  << mNumBlocks.load() << std::endl;
    }

    mPullCount.fetch_add(1);



    if (ptr) {
        /* ptr-------->ptr                  */       
        /* ��            ��                   */       
        /* [SLIST_ENTRY][ ... memory ... ]  */
        return reinterpret_cast<void*>(reinterpret_cast<char*>(ptr) + sizeof(SLIST_ENTRY));
    }
    else {
        /* ������ ����.. */
        AddMemory();
        mNumBlocks.fetch_sub(1);
        PSLIST_ENTRY ptr = ::InterlockedPopEntrySList(&mSListHeader);
        return reinterpret_cast<void*>(reinterpret_cast<char*>(ptr) + sizeof(SLIST_ENTRY));

    }


    return nullptr; // ��� ������ ����� ���� ���
}

void SListMemoryPool::Push(void* ptr)
{
    mPushCount.fetch_add(1);
    //std::cout << TLS_MGR->Get_TlsSendBufFactory()->SendBufFactory << "�ݳ� ���̵� : " << TLS_MGR->Get_TlsInfoData()->id << " �޸� ������ - " << ptr << " PUSHCNT : " << mPushCount.load() << std::endl;

    mNumBlocks.fetch_add(1);
    //std::cout << TLS_MGR->Get_TlsSendBufFactory()->SendBufFactory << "�ݳ� ���̵� : " << TLS_MGR->Get_TlsInfoData()->id << " �޸� ������ - "  << ptr << " ���� : " << mNumBlocks.load() << std::endl;


    // SLIST�� �޸� ��� �߰�
        /* ptr<--------ptr                  */       
        /* ��            ��                   */       
        /* [SLIST_ENTRY][ ... memory ... ]  */
    ::InterlockedPushEntrySList(&mSListHeader, reinterpret_cast<PSLIST_ENTRY>(reinterpret_cast<char*>(ptr) - sizeof(SLIST_ENTRY)));
}

void SListMemoryPool::AddMemory()
{
    // SLIST�� �޸� ��� �߰�
    /* [SLIST_ENTRY][ ... memory ... ] */
    SListMemoryBlock* ptr = static_cast<SListMemoryBlock*>(::_aligned_malloc(sizeof(SLIST_ENTRY) + mMemorySize, MEMORY_ALLOCATION_ALIGNMENT));
    if (ptr) {
        mNumBlocks.fetch_add(1);
        InterlockedPushEntrySList(&mSListHeader, reinterpret_cast<PSLIST_ENTRY>(ptr));
    }

}
