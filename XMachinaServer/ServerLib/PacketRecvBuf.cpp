#include "pch.h"
#include "PacketRecvBuf.h"
#include "MemoryManager.h"
#include "SListMemoryPool.h"



PacketRecvBuf::PacketRecvBuf(UINT32 bufSize)
	: mBufferSize(bufSize)
{
	mCapacity = bufSize * static_cast<UINT32>(PacketRecvBuf::Info::bufferCount);

	// Use SListMemoryPool
	const size_t MemoryBlockSize	 = mCapacity;
	const size_t NumBlock			 = 1;
	MEMORY->AddSListMemoryPool("RecvBuf", MemoryBlockSize, NumBlock);

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
		// �� ��ħ �б�+���� Ŀ���� ������ ��ġ���, �� �� ����.
		mReadCursor = mWriteCursor = 0;
	}
	else {

		// ���� ������ ���� 1�� ũ�� �̸��̸�, �����͸� ������ �����.
		// ���� ���� ũ�� < ���� ���� ũ��
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
