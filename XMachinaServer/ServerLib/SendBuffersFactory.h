#pragma once

/// +-----------------------------------------------
///					 SendBuffersFactory 
/// ________________________________________________
/// 
/// SendPktBuf  ( SLIST )
/// ________________________________________________
/// 
/// Variable Length PktBuf				
/// [32] [64] [128] [256] [512]
/// [32] [64] [128] [256] [512]
/// ...  ...  ...   ...   ...
/// [32] [64] [128] [256] [512] 
/// ________________________________________________
/// Fixed Length PktBuf
/// 
/// -----------------------------------------------+


namespace SendPktInfo {
	enum class Type {
		Variable_Length,
		Fixed_Length
	};

	/* Variable - 크기에 맞는 바이트 메모리 풀에 가변길이 데이터를 Write */
	enum class Var : UINT8 {
		BYTES_32,
		BYTES_64,
		BYTES_128,
		BYTES_256,
		BYTES_512,
	};
	/* Fixed */
	enum class Fix : UINT8 {
		LogIn,
		Transform,

		// ... 
	};

	constexpr UINT16 MemoryNum = 50;
}


// 각 쓰레드 마다 하나씩 생성 시킬 것 ( 안전하게 shared_ptr 로 생성 )
class SendBuffersFactory : public std::enable_shared_from_this<SendBuffersFactory>
{
private:
	std::unordered_map<SendPktInfo::Var, class SListMemoryPool*> mMemPools_VarPkt = {}; // 가변길이 패킷 전용 메모리 풀 
	std::unordered_map<SendPktInfo::Fix, class SListMemoryPool*> mMemPools_FixPkt = {}; // 고정길이 패킷 전용 메모리 풀 

public:
	SendBuffersFactory();
	~SendBuffersFactory();

public:
	void InitPacketMemoryPools();

public:
	/* 메모리 풀에서 메모리를 가져온다. */ 
	void* Pull_VarPkt(size_t memorySize);
	void* Pull_FixPkt(SendPktInfo::Fix type);

	/*  메모리 풀에 메모리를 반납한다. */
	void  Push_VarPkt(size_t memorySize, void* ptr);
	void  Push_FixPkt(SendPktInfo::Fix type, void* ptr); 


};

