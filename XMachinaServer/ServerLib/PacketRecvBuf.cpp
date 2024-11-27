#include "pch.h"
#include "PacketRecvBuf.h"
#include "MemoryManager.h"
#include "PacketHeader.h"

PacketRecvBuf::PacketRecvBuf(UINT32 bufSize)
    : mBufferSize(bufSize)
{
    mCapacity = bufSize * static_cast<UINT32>(PacketRecvBuf::Info::bufferCount);

    /* SListMemoryPool 추가 */
    const size_t MemoryBlockSize = mCapacity;

    // 객체의 주소를 문자열로 변환하여 유니크한 이름 생성

    //std::string uniqueName = "RecvBuf_" + std::to_string(reinterpret_cast<std::uintptr_t>(this));
    //std::string uniqueName = "RecvBuf_" + std::to_string(id);
    //MEMORY->AddMemoryPool(uniqueName.c_str(), MemoryBlockSize, 1);

    /* RecvBuf MemoryBlock 사용 */
    mBuffer.reserve(MemoryBlockSize);
    //void* blockPtr = MEMORY->Allocate(uniqueName.c_str());
    void* blockPtr = new BYTE[MemoryBlockSize]; // 동적 메모리 할당
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
    // 객체의 주소를 문자열로 변환하여 동일한 이름 사용
    delete[] static_cast<BYTE*>(mReturnBlockPtr); // 할당된 메모리 해제
    
    std::string uniqueName = "RecvBuf_" + std::to_string(reinterpret_cast<std::uintptr_t>(this));
   // MEMORY->Free(uniqueName.c_str(), mReturnBlockPtr);
}

void PacketRecvBuf::Clean()
{
	if (mDataSize == 0) {
		// 읽기 및 쓰기 인덱스를 초기화
		mReadPointer = mWritePointer = 0;
	}
}

bool PacketRecvBuf::OnRead(UINT32 bytes)
{
	if (bytes > mDataSize)
		return false;

	IncrementIndex(mReadPointer, bytes);
	mDataSize -= bytes;
	return true;
}

bool PacketRecvBuf::OnWrite(UINT32 bytes)
{
	if (bytes > GetFreeSize())
		return false;

	IncrementIndex(mWritePointer, bytes);
	mDataSize += bytes;
	return true;
}

void PacketRecvBuf::IncrementIndex(UINT32& index, UINT32 numOfBytes)
{
	index = (index + numOfBytes) % mCapacity;
}

UINT32 PacketRecvBuf::GetFreeSize()
{
	if (mWritePointer >= mReadPointer) {
		return mCapacity - mWritePointer + mReadPointer - 1; // -1 to avoid overwrite
	}
	else {
		return mReadPointer - mWritePointer - 1;
	}
}