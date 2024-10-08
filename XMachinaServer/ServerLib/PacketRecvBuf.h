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
		Size = 0x50'000 // 64KB
	};

private:
	std::vector<BYTE> mBuffer;
	UINT32 mCapacity   = 0;
	UINT32 mBufferSize = 0;
	UINT32 mRead_Idx   = 0;
	UINT32 mWrite_Idx  = 0;
	UINT32 mDataSize   = 0;  // 현재 데이터 크기

	void* mReturnBlockPtr = nullptr;

public:
	PacketRecvBuf() = default;
	PacketRecvBuf(UINT32 bufSize);
	~PacketRecvBuf();

public:
	void Clean();
	bool OnRead(UINT32 numOfBytes); 
	bool OnWrite(UINT32 numOfBytes);

	BYTE*   GetReadPos()    { return &mBuffer[mRead_Idx]; }
	BYTE*   GetWritePos()   { return &mBuffer[mWrite_Idx]; }
	UINT32	GetDataSize()   { return mDataSize; }
	UINT32	GetFreeSize();


private:
	void IncrementIndex(UINT32& index, UINT32 numOfBytes);
};