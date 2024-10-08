#include "pch.h"
#include "PacketRecvBuf.h"
#include "MemoryManager.h"
#include "SListMemoryPool.h"
#include "PacketHeader.h"

PacketRecvBuf::PacketRecvBuf(UINT32 bufSize)
    : mBufferSize(bufSize)
{
    mCapacity = bufSize * static_cast<UINT32>(PacketRecvBuf::Info::bufferCount);

    /* SListMemoryPool �߰� */
    const size_t MemoryBlockSize = mCapacity;

    // ��ü�� �ּҸ� ���ڿ��� ��ȯ�Ͽ� ����ũ�� �̸� ����
    std::string uniqueName = "RecvBuf_" + std::to_string(reinterpret_cast<std::uintptr_t>(this));
    MEMORY->AddMemoryPool(uniqueName.c_str(), MemoryBlockSize, 1);

    /* RecvBuf MemoryBlock ��� */
    mBuffer.reserve(MemoryBlockSize);
    void* blockPtr = MEMORY->Allocate(uniqueName.c_str());
    mReturnBlockPtr = blockPtr;

    if (blockPtr) {
        ZeroMemory(blockPtr, MemoryBlockSize);
        BYTE* blockBytes = static_cast<BYTE*>(blockPtr);
        mBuffer.insert(mBuffer.end(), blockBytes, blockBytes + MemoryBlockSize);
    }
    else {
        std::cout << "Failed To Allocate Memory Block : PacketRecvBuf\n";
    }
}

PacketRecvBuf::~PacketRecvBuf()
{
    // ��ü�� �ּҸ� ���ڿ��� ��ȯ�Ͽ� ������ �̸� ���
    std::string uniqueName = "RecvBuf_" + std::to_string(reinterpret_cast<std::uintptr_t>(this));
    MEMORY->Free(uniqueName.c_str(), mReturnBlockPtr);
}

void PacketRecvBuf::Clean()
{
	if (mDataSize == 0) {
		// �б� �� ���� �ε����� �ʱ�ȭ
		mRead_Idx = mWrite_Idx = 0;
	}
}

bool PacketRecvBuf::OnRead(UINT32 numOfBytes)
{
	if (numOfBytes > mDataSize)
		return false;

	IncrementIndex(mRead_Idx, numOfBytes);
	mDataSize -= numOfBytes;
	return true;
}

bool PacketRecvBuf::OnWrite(UINT32 numOfBytes)
{
	if (numOfBytes > GetFreeSize())
		return false;

	IncrementIndex(mWrite_Idx, numOfBytes);
	mDataSize += numOfBytes;
	return true;
}

void PacketRecvBuf::IncrementIndex(UINT32& index, UINT32 numOfBytes)
{
	index = (index + numOfBytes) % mCapacity;
}

UINT32 PacketRecvBuf::GetFreeSize()
{
	if (mWrite_Idx >= mRead_Idx) {
		return mCapacity - mWrite_Idx + mRead_Idx - 1; // -1 to avoid overwrite
	}
	else {
		return mRead_Idx - mWrite_Idx - 1;
	}
}