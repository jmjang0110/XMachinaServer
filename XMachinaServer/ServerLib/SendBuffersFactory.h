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

	/* Variable - ũ�⿡ �´� ����Ʈ �޸� Ǯ�� �������� �����͸� Write */
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


// �� ������ ���� �ϳ��� ���� ��ų �� ( �����ϰ� shared_ptr �� ���� )
class SendBuffersFactory : public std::enable_shared_from_this<SendBuffersFactory>
{
private:
	std::unordered_map<SendPktInfo::Var, class SListMemoryPool*> mMemPools_VarPkt = {}; // �������� ��Ŷ ���� �޸� Ǯ 
	std::unordered_map<SendPktInfo::Fix, class SListMemoryPool*> mMemPools_FixPkt = {}; // �������� ��Ŷ ���� �޸� Ǯ 

public:
	SendBuffersFactory();
	~SendBuffersFactory();

public:
	void InitPacketMemoryPools();

public:
	/* �޸� Ǯ���� �޸𸮸� �����´�. */ 
	void* Pull_VarPkt(size_t memorySize);
	void* Pull_FixPkt(SendPktInfo::Fix type);

	/*  �޸� Ǯ�� �޸𸮸� �ݳ��Ѵ�. */
	void  Push_VarPkt(size_t memorySize, void* ptr);
	void  Push_FixPkt(SendPktInfo::Fix type, void* ptr); 


};

