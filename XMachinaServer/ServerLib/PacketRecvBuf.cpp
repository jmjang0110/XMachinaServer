#include "pch.h"
#include "PacketRecvBuf.h"
#include "MemoryManager.h"
#include "MemoryPool.h"



PacketRecvBuf::PacketRecvBuf(UINT32 bufSize)
	: mBufferSize(bufSize)
{
	mCapacity = bufSize * static_cast<UINT32>(PacketRecvBuf::Info::bufferCount);

	// Use MemoryPool
	const size_t MemoryBlockSize	 = mCapacity;
	const size_t NumBlock			 = 1;
	MEMORY->AddMemoryPool(MemoryBlockSize, NumBlock);

	mBuffer.reserve(MemoryBlockSize);
	void* block = MEMORY->Allocate(MemoryBlockSize);
	if (block) {
		BYTE* blockBytes = static_cast<BYTE*>(block);
		mBuffer.insert(mBuffer.end(), blockBytes, blockBytes + MemoryBlockSize);
	}
	else {
		std::cout << "Failed To Allocate Memory Block : PacketRecvBuf\n";
	}

}

PacketRecvBuf::~PacketRecvBuf()
{
	
}

void PacketRecvBuf::Clean()
{
	INT32 Datasize = GetDataSize();
	if (Datasize == 0) {
		// 딱 마침 읽기+쓰기 커서가 동일한 위치라면, 둘 다 리셋.
		mReadCursor = mWriteCursor = 0;
	}
	else {

		// 여유 공간이 버퍼 1개 크기 미만이면, 데이터를 앞으로 땅긴다.
		// 여유 버퍼 크기 < 단일 버퍼 크기
		if (GetFreeSize() < mBufferSize) {
			::memcpy(&mBuffer[0], &mBuffer[mReadCursor], Datasize);
			mReadCursor = 0;
			mWriteCursor = Datasize;
		}
	}
}

bool PacketRecvBuf::OnRead(UINT32 numOfBytes)
{
	if (numOfBytes > GetDataSize())
		return false;

	mReadCursor += numOfBytes;
	return true;
}

bool PacketRecvBuf::OnWrite(UINT32 numOfBytes)
{
	if (numOfBytes > GetFreeSize())
		return false;

	mWriteCursor += numOfBytes;
	return true;
}
