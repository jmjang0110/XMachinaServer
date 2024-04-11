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
	/* �޸� �ݳ� */
	FRAMEWORK->GetSendFactory()->Push_VarPkt(mMemoryPoolSize, mPtrFromMemPool); /* �޸� Ǯ�� �ݳ�! */
}

void PacketSendBuf::Close(UINT32 writeSize)
{

}

