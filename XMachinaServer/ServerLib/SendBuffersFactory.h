#pragma once

/// +-----------------------------------------------
///					 SendBuffersFactory 
/// ________________________________________________
/// 
/// SendPktBuf  ( SLIST )
/// ________________________________________________
/// 
/// 각 Thread 별로 SendBufferFactory 를 소유하고 있다. ( TLS ) 
/// Variable Length PktBuf				
/// [32] [64] [128] [256] [512]
/// [32] [64] [128] [256] [512]
/// ...  ...  ...   ...   ...
/// [32] [64] [128] [256] [512] 
/// ________________________________________________
/// Fixed Length PktBuf
/// 
/// 
/// - TLS 영역에 생성시킬것이기에 싱글쓰레드처럼 사용가능
/// -----------------------------------------------+


namespace SendPktInfo {
	enum class Type {
		SendPacket,
		Variable_Length, // 가변 길이 Send Packet 
		Fixed_Length	 // 고정 길이 Send Packet 
	};

	/* Variable - 크기에 맞는 바이트 메모리 풀에 가변길이 데이터를 Write */
	enum class Var : UINT16 {
		BYTES_32 = 32,
		BYTES_64 = 64,
		BYTES_128 = 128,
		BYTES_256 = 256,
		BYTES_512 = 512,
		BYTES_1024 = 1024,
		BYTES_2048 = 2048,
		BYTES_4096 = 4096,
	};
	/* Fixed */
	enum class Fix : UINT8 {
		LogIn,
		Transform,

		// ... 
	};

	constexpr UINT16 MemoryNum = 200;
}


// 각 쓰레드 마다 하나씩 생성 시킬 것 ( 안전하게 shared_ptr 로 생성 )
class SendBuffersFactory : public std::enable_shared_from_this<SendBuffersFactory>
{
private:
	SPtr<ServerMemoryPool>									   mMemPools_SptrSendPkt = {}; // SendPkt 메모리 풀 
	std::unordered_map<SendPktInfo::Var, SPtr<ServerMemoryPool>> mMemPools_VarPkt      = {}; // 가변길이 패킷 전용 메모리 풀 
	std::unordered_map<SendPktInfo::Fix, SPtr<ServerMemoryPool>> mMemPools_FixPkt      = {}; // 고정길이 패킷 전용 메모리 풀 

	std::atomic_int mPushCount = 0;;
	std::atomic_int mPullCount = 0;;
public:
	SendBuffersFactory();
	~SendBuffersFactory();

public:
	void InitPacketMemoryPools();

public:
	/* 메모리 풀에서 메모리를 가져온다. */ 
	void* Pull_SendPkt();
	void* Pull_VarPkt(size_t memorySize);
	void* Pull_FixPkt(SendPktInfo::Fix type);

	/*  메모리 풀에 메모리를 반납한다. */
	bool  Push_VarPkt(size_t memorySize, void* ptr);
	bool  Push_FixPkt(SendPktInfo::Fix type, void* ptr);
	bool  Push_SendPkt(void* ptr);


	SPtr_PacketSendBuf CreateVarSendPacketBuf(const uint8_t* bufPtr, const uint16_t SerializedDataSize, uint16_t ProtocolId, size_t memorySize);
	SPtr_PacketSendBuf CreateFixSendPacketBuf(SendPktInfo::Fix pktDataType);

	void AddMemoryPool(SendPktInfo::Var memoryBlockSize, int cnt);

public:
	/// +---------------------
	///	 CREATE SERVER PACKET
	/// ---------------------+
	SPtr_PacketSendBuf CreatePacket(const uint8_t* bufPtr, const uint16_t SerializedDataSize, uint16_t ProtocolId);


	/// +-------------------------
	///	 SEND BUF MEMORY OPERATOR
	/// -------------------------+
public:
	static PacketSendBuf* New(void* dst, BYTE* ptr, UINT16 memsize, BYTE* buffer, UINT32 allocSize);
	template<typename Type>
	static void Delete(Type* ptr);
	
	std::shared_ptr<PacketSendBuf> Make_Shared(void* dst, BYTE* ownerptr, UINT16 memsize, BYTE* buffer, UINT32 allocSize);

};

template<typename Type>
inline void SendBuffersFactory::Delete(Type* ptr)
{
	ptr->~Type();
}

inline PacketSendBuf* SendBuffersFactory::New(void* dst, BYTE* ptr, UINT16 memsize, BYTE* buffer, UINT32 allocSize)
{
	PacketSendBuf* Memory = static_cast<PacketSendBuf*>(static_cast<void*>(dst));
	new(Memory)PacketSendBuf(ptr, memsize, buffer, allocSize); /* Placement New */
	return Memory;;
}

/* dst에 메모리를 쓴다... */
inline std::shared_ptr<PacketSendBuf> SendBuffersFactory::Make_Shared(void* dst, BYTE* ownerptr, UINT16 memsize, BYTE* buffer, UINT32 allocSize)
{
	return std::shared_ptr<PacketSendBuf>{ SendBuffersFactory::New(dst, ownerptr, memsize, buffer, allocSize), SendBuffersFactory::Delete<PacketSendBuf>};
}
