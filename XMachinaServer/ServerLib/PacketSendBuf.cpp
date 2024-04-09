#include "pch.h"
#include "PacketSendBuf.h"
#include "SListMemoryPool.h"
#include "SendBuffersFactory.h"
#include "../ServerLib/ThreadManager.h"
#include "../Framework.h"


PacketSendBuf::PacketSendBuf(BYTE* buffer, UINT32 allocSize)
	: mBuffer(buffer), mBufferTotalSize(allocSize)
{
}

PacketSendBuf::~PacketSendBuf()
{
	/* 메모리 반납 */
	if (TLS_MGR->Get_TlsInfoData()) {
		//std::cout << "반납 아이디 : " << TLS_MGR->Get_TlsInfoData()->id << " 메모리 포인터 - " << mPtrFromMemPool << std::endl;

	}
	//TLS_MGR->Get_TlsSendBufFactory()->SendBufFactory->Push_VarPkt(mMemoryPoolSize, mPtrFromMemPool);
	FRAMEWORK->GetSendFactory()->Push_VarPkt(mMemoryPoolSize, mPtrFromMemPool);

}

void PacketSendBuf::Close(UINT32 writeSize)
{
	// todo : test 지우기
	TLS_MGR->Get_TlsSendBufFactory()->SendBufFactory->Push_VarPkt(mMemoryPoolSize, mPtrFromMemPool);

}

