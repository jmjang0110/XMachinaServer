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
	FRAMEWORK->GetSendFactory()->Push_VarPkt(mMemoryPoolSize, mPtrFromMemPool); /* 메모리 풀에 반납! */
}

void PacketSendBuf::Close(UINT32 writeSize)
{

}

