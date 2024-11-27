#include "pch.h"
#include "PacketSendBuf.h"
#include "SendBuffersFactory.h"
#include "../ServerLib/ThreadManager.h"
#include "../Framework.h"


PacketSendBuf::PacketSendBuf(BYTE* ptrMem, UINT16 MemSize, BYTE* buffer, UINT32 allocSize)
	: mPtrFromMemPool(ptrMem)
	, mMemoryPoolSize(MemSize)
	, mBuffer(buffer)
	, mBufferTotalSize(allocSize)
{
	int i = 0;

}

PacketSendBuf::~PacketSendBuf()
{
	/* �޸� �ݳ� */

	//LOG_MGR->Cout("THIS : ", this, "\n");
	//LOG_MGR->Cout("MEMO : ", mPtrFromMemPool, "\n");

	void* MemPtr = static_cast<BYTE*>(mBuffer) - ((mMemoryPoolSize - mBufferTotalSize));
	if (!FRAMEWORK->GetSendFactory()->Push_VarPkt(mMemoryPoolSize, MemPtr)) /* �޸� Ǯ�� �ݳ�! */
		delete MemPtr;
	FRAMEWORK->GetSendFactory()->Push_SendPkt(reinterpret_cast<void*>(this));
	//FRAMEWORK->GetSendFactory()->Push_SendPkt(mPtrFromMemPool);

	//LOG_MGR->Cout("MemPtr : ", MemPtr, " RETURN\n");
}



void PacketSendBuf::SetPacketHederInfo(PacketHeader info)
{
	
}

void PacketSendBuf::SetBuffer(BYTE* buf)
{
	mBuffer = buf;
}
