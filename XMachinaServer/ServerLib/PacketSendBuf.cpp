#include "pch.h"
#include "PacketSendBuf.h"
#include "SListMemoryPool.h"
#include "SendBuffersFactory.h"
#include "../ServerLib/ThreadManager.h"
#include "../Framework.h"


PacketSendBuf::PacketSendBuf(BYTE* ptrMem, UINT16 MemSize, BYTE* buffer, UINT32 allocSize)
	: mPtrFromMemPool(ptrMem)
	, mMemoryPoolSize(MemSize)
	, mBuffer(buffer)
	, mBufferTotalSize(allocSize)
{
}

PacketSendBuf::~PacketSendBuf()
{
	/* 메모리 반납 */
	void* MemPtr = static_cast<BYTE*>(mBuffer) - ((mMemoryPoolSize - mBufferTotalSize));
	FRAMEWORK->GetSendFactory()->Push_VarPkt(mMemoryPoolSize, MemPtr); /* 메모리 풀에 반납! */
	FRAMEWORK->GetSendFactory()->Push_SendPkt(this);
}



void PacketSendBuf::SetPacketHederInfo(PacketHeader info)
{
	
}

void PacketSendBuf::SetBuffer(BYTE* buf)
{
	mBuffer = buf;
}
