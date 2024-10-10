#pragma once

/// +-----------------------------------------------
///					 SendBuffersFactory 
/// ________________________________________________
/// 
/// SendPktBuf  ( SLIST )
/// ________________________________________________
/// 
/// �� Thread ���� SendBufferFactory �� �����ϰ� �ִ�. ( TLS ) 
/// Variable Length PktBuf				
/// [32] [64] [128] [256] [512]
/// [32] [64] [128] [256] [512]
/// ...  ...  ...   ...   ...
/// [32] [64] [128] [256] [512] 
/// ________________________________________________
/// Fixed Length PktBuf
/// 
/// 
/// - TLS ������ ������ų���̱⿡ �̱۾�����ó�� ��밡��
/// -----------------------------------------------+


namespace SendPktInfo {
	enum class Type {
		SendPacket,
		Variable_Length, // ���� ���� Send Packet 
		Fixed_Length	 // ���� ���� Send Packet 
	};

	/* Variable - ũ�⿡ �´� ����Ʈ �޸� Ǯ�� �������� �����͸� Write */
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


// �� ������ ���� �ϳ��� ���� ��ų �� ( �����ϰ� shared_ptr �� ���� )
class SendBuffersFactory : public std::enable_shared_from_this<SendBuffersFactory>
{
private:
	SPtr<ServerMemoryPool>									   mMemPools_SptrSendPkt = {}; // SendPkt �޸� Ǯ 
	std::unordered_map<SendPktInfo::Var, SPtr<ServerMemoryPool>> mMemPools_VarPkt      = {}; // �������� ��Ŷ ���� �޸� Ǯ 
	std::unordered_map<SendPktInfo::Fix, SPtr<ServerMemoryPool>> mMemPools_FixPkt      = {}; // �������� ��Ŷ ���� �޸� Ǯ 

	std::atomic_int mPushCount = 0;;
	std::atomic_int mPullCount = 0;;
public:
	SendBuffersFactory();
	~SendBuffersFactory();

public:
	void InitPacketMemoryPools();

public:
	/* �޸� Ǯ���� �޸𸮸� �����´�. */ 
	void* Pull_SendPkt();
	void* Pull_VarPkt(size_t memorySize);
	void* Pull_FixPkt(SendPktInfo::Fix type);

	/*  �޸� Ǯ�� �޸𸮸� �ݳ��Ѵ�. */
	void  Push_VarPkt(size_t memorySize, void* ptr);
	void  Push_FixPkt(SendPktInfo::Fix type, void* ptr); 
	void  Push_SendPkt(void* ptr);


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

/* dst�� �޸𸮸� ����... */
inline std::shared_ptr<PacketSendBuf> SendBuffersFactory::Make_Shared(void* dst, BYTE* ownerptr, UINT16 memsize, BYTE* buffer, UINT32 allocSize)
{
	return std::shared_ptr<PacketSendBuf>{ SendBuffersFactory::New(dst, ownerptr, memsize, buffer, allocSize), SendBuffersFactory::Delete<PacketSendBuf>};
}
