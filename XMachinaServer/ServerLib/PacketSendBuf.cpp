#include "pch.h"
#include "PacketSendBuf.h"
#include "SListMemoryPool.h"


PacketSendBuf::PacketSendBuf(BYTE* buffer, UINT32 allocSize)
	: mBuffer(buffer), mAllocSize(allocSize)
{
}

PacketSendBuf::~PacketSendBuf()
{

}

void PacketSendBuf::Close(UINT32 writeSize)
{
}

