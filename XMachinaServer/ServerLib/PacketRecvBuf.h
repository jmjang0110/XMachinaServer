#pragma once

#pragma once
/// +-----------------------------------------------
///		  packetRecvBuf - Ring Buffer Version
/// ________________________________________________

class PacketRecvBuf
{
public:
	enum class Info {
		bufferCount = 10,
		Size = 0x500 
	};

private:
	std::vector<BYTE> mBuffer;
	UINT32 mCapacity      = 0;
	UINT32 mBufferSize    = 0;
	UINT32 mReadPointer   = 0;
	UINT32 mWritePointer  = 0;
	UINT32 mDataSize      = 0;  // 현재 데이터 크기

	void* mReturnBlockPtr = nullptr;

public:
	PacketRecvBuf() = default;
	PacketRecvBuf(UINT32 bufSize);
	~PacketRecvBuf();

public:
	void Clean();
	bool OnRead(UINT32 bytes); 
	bool OnWrite(UINT32 bytes);

	BYTE*   GetReadPointer()    { return &mBuffer[mReadPointer]; }
	BYTE*   GetWritePointer()   { return &mBuffer[mWritePointer]; }
	UINT32	GetDataSize()		{ return mDataSize; }
	UINT32	GetFreeSize();


private:
	void IncrementIndex(UINT32& index, UINT32 numOfBytes);
};