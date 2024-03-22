#pragma once


/// +-----------------------------------------------
///				      PacketRecvBuf 
/// ________________________________________________

/// -----------------------------------------------+

class PacketRecvBuf
{
public:
	enum class Info {
		bufferCount = 10,
		Size        = 0x10'000 // 64kb
	};

private:
	std::vector<BYTE> mBuffer = {};

	UINT32 mCapacity          = 0;
	UINT32 mBufferSize        = 0;
	UINT32 mReadCursor        = 0;
	UINT32 mWriteCursor       = 0;
public:
	PacketRecvBuf() = default;
	PacketRecvBuf(UINT32 bufSize);
	~PacketRecvBuf();

public:
	void Clean();
	bool OnRead(UINT32 numOfBytes);
	bool OnWrite(UINT32 numOfBytes);

	BYTE*	GetReadPos()  { return &mBuffer[mReadCursor]; }
	BYTE*	GetWritePos() { return &mBuffer[mWriteCursor]; }
	UINT32	GetDataSize() { return mWriteCursor - mReadCursor; }
	UINT32	GetFreeSize() { return mCapacity - mWriteCursor; }

};

