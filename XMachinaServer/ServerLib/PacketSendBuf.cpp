#include "pch.h"
#include "PacketSendBuf.h"
#include "SListMemoryPool.h"
#include "SendBuffersFactory.h"
#include "../ServerLib/ThreadManager.h"


PacketSendBuf::PacketSendBuf(BYTE* buffer, UINT32 allocSize)
	: mBuffer(buffer), mBufferTotalSize(allocSize)
{
}

PacketSendBuf::~PacketSendBuf()
{
	/* �޸� �ݳ� */
	if (TLS_MGR->Get_TlsInfoData()) {
		//std::cout << "�ݳ� ���̵� : " << TLS_MGR->Get_TlsInfoData()->id << " �޸� ������ - " << mPtrFromMemPool << std::endl;

	}
	TLS_MGR->Get_TlsSendBufFactory()->SendBufFactory->Push_VarPkt(mMemoryPoolSize, mPtrFromMemPool);

}

void PacketSendBuf::Close(UINT32 writeSize)
{
}

