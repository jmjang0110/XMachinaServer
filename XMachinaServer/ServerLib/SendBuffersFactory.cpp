#include "pch.h"
#include "SendBuffersFactory.h"
#include "SListMemoryPool.h"
#include "MemoryManager.h"
#include "../Framework.h"

#undef max
#include <flatbuffers/flatbuffers.h>
#include "Protocol/FBProtocol_generated.h"

SendBuffersFactory::SendBuffersFactory()
{

}

SendBuffersFactory::~SendBuffersFactory()
{
	for (auto& pair : mMemPools_VarPkt) {
		delete pair.second;
	}
	for (auto& pair : mMemPools_FixPkt) {
		delete pair.second;
	}
	
	mMemPools_VarPkt.clear();
	mMemPools_FixPkt.clear();

}

void SendBuffersFactory::InitPacketMemoryPools()
{
	{
		SListMemoryPool* pool = new SListMemoryPool(sizeof(PacketSendBuf));
		mMemPools_SptrSendPkt = pool;
		for (size_t i = 0; i < 100; ++i) {
			mMemPools_SptrSendPkt->AddMemory();
		}
	}

/// +-------------------------
///	  Variable Length Buffer
/// -------------------------+

	/* BYTES_32 */
	{
		const size_t MemoryBlockSize = 32;
		SListMemoryPool* Pool = new SListMemoryPool(MemoryBlockSize);
		mMemPools_VarPkt[SendPktInfo::Var::BYTES_32] = Pool;
		for (UINT16 i = 1; i < SendPktInfo::MemoryNum; ++i) {
			mMemPools_VarPkt[SendPktInfo::Var::BYTES_32]->AddMemory();
		}
	}
	/* BYTES_64 */
	{
		const size_t MemoryBlockSize = 64;
		SListMemoryPool* Pool = new SListMemoryPool(MemoryBlockSize);
		mMemPools_VarPkt[SendPktInfo::Var::BYTES_64] = Pool;
		for (UINT16 i = 1; i < SendPktInfo::MemoryNum; ++i) {
			mMemPools_VarPkt[SendPktInfo::Var::BYTES_64]->AddMemory();
		}
	}

	/* BYTES_128 */
	{
		const size_t MemoryBlockSize = 128;
		SListMemoryPool* Pool = new SListMemoryPool(MemoryBlockSize);
		mMemPools_VarPkt[SendPktInfo::Var::BYTES_128] = Pool;
		for (UINT16 i = 1; i < SendPktInfo::MemoryNum; ++i) {
			mMemPools_VarPkt[SendPktInfo::Var::BYTES_128]->AddMemory();
		}
	}

	/* BYTES_256 */
	{
		const size_t MemoryBlockSize = 256;
		SListMemoryPool* Pool = new SListMemoryPool(MemoryBlockSize);
		mMemPools_VarPkt[SendPktInfo::Var::BYTES_256] = Pool;
		for (UINT16 i = 1; i < SendPktInfo::MemoryNum; ++i) {
			mMemPools_VarPkt[SendPktInfo::Var::BYTES_256]->AddMemory();
		}
	}

	/* BYTES_512 */
	{
		const size_t MemoryBlockSize = 512;
		SListMemoryPool* Pool = new SListMemoryPool(MemoryBlockSize);
		mMemPools_VarPkt[SendPktInfo::Var::BYTES_512] = Pool;
		for (UINT16 i = 1; i < SendPktInfo::MemoryNum; ++i) {
			mMemPools_VarPkt[SendPktInfo::Var::BYTES_512]->AddMemory();
		}
	}

	/// +-------------------------
	///	  Variable Length Buffer
	/// -------------------------+
		/* LogIn */
		/*{
			const size_t MemoryBlockSize = sizeof(LogIn);
			const size_t NumBlock = 1;
			SListMemoryPool* Pool = new SListMemoryPool(MemoryBlockSize, NumBlock);
			mMemPools_VarPkt[SendPktInfo::Fix::LogIn] = Pool;
			for (UINT16 i = 1; i < SendPktInfo::MemoryNum; ++i) {
				SListMemoryPool* Pool = new SListMemoryPool(MemoryBlockSize, NumBlock);
				mMemPools_VarPkt[SendPktInfo::Fix::LogIn]->AddMemory();
			}
		}*/

}

void* SendBuffersFactory::Pull_SendPkt()
{
	return mMemPools_SptrSendPkt->Pull();
}

/* PULL */
void* SendBuffersFactory::Pull_VarPkt(size_t memorySize)
{
	if (memorySize <= 32) {
		return mMemPools_VarPkt[SendPktInfo::Var::BYTES_32]->Pull();
	}
	else if (32 < memorySize && memorySize <= 64) {
		return mMemPools_VarPkt[SendPktInfo::Var::BYTES_64]->Pull();
	}
	else if (64 < memorySize && memorySize <= 128){
		return mMemPools_VarPkt[SendPktInfo::Var::BYTES_128]->Pull();
	}
	else if (128 < memorySize && memorySize <= 256){
		return mMemPools_VarPkt[SendPktInfo::Var::BYTES_256]->Pull();
	}
	else if (256 < memorySize && memorySize <= 512){
		return mMemPools_VarPkt[SendPktInfo::Var::BYTES_512]->Pull();
	}

	return nullptr;
}

void* SendBuffersFactory::Pull_FixPkt(SendPktInfo::Fix type)
{
	return mMemPools_FixPkt[type]->Pull();
}


/* PUSH */
void SendBuffersFactory::Push_VarPkt(size_t memorySize, void* ptr)
{
	if (memorySize <= 32) {
		mMemPools_VarPkt[SendPktInfo::Var::BYTES_32]->Push(ptr);
	}
	else if (32 < memorySize && memorySize <= 64) {
		mMemPools_VarPkt[SendPktInfo::Var::BYTES_64]->Push(ptr);
	}
	else if (64 < memorySize && memorySize <= 128) {
		mMemPools_VarPkt[SendPktInfo::Var::BYTES_128]->Push(ptr);
	}
	else if (128 < memorySize && memorySize <= 256) {
		mMemPools_VarPkt[SendPktInfo::Var::BYTES_256]->Push(ptr);
	}
	else if (256 < memorySize && memorySize <= 512) {
		mMemPools_VarPkt[SendPktInfo::Var::BYTES_512]->Push(ptr);
	}
}

void SendBuffersFactory::Push_FixPkt(SendPktInfo::Fix type, void* ptr)
{
	mMemPools_FixPkt[type]->Push(ptr);
}

void SendBuffersFactory::Push_SendPkt(void* ptr)
{
	mMemPools_SptrSendPkt->Push(ptr);
}

SPtr_PacketSendBuf SendBuffersFactory::CreateVarSendPacketBuf(const uint8_t* bufPtr, const uint16_t SerializedDataSize, uint16_t ProtocolId, size_t memorySize)
{

	/* 메모리 풀에서 메모리를 받아와서 shared_ptr 로 SendPacket Buffer 를 만들어 반환하자! */
	
	UINT8 offsetMemSize = 0;
	if (memorySize <= 32) {
		offsetMemSize = 32 - memorySize;
	}
	else if (memorySize <= 64) {
		offsetMemSize = 64 - memorySize;
	}
	else if (memorySize <= 128) {
		offsetMemSize = 128 - memorySize;
	}
	else if (memorySize <= 256) {
		offsetMemSize = 256 - memorySize;
	}
	else if (memorySize <= 512) {
		offsetMemSize = 512 - memorySize;
	}
	if (offsetMemSize < 0)
		return nullptr;


	/*               실질적으로 쓰는 메모리								 */		
	/*                      ↓ 					                         */		
	/* [     offset = 28    [ memsize = 4 ]] - 32byte 메모리에 4바이트만 사용  */
	
	/* 실직적으로 사용하는 메모리의 시작위치로 이동 */
 	BYTE* ptr                  = reinterpret_cast<BYTE*>(Pull_VarPkt(memorySize));
	BYTE* StartPtr             = ptr + offsetMemSize;
	SPtr_PacketSendBuf sendBuf = Make_Shared(Pull_SendPkt(), ptr, memorySize + offsetMemSize, StartPtr, memorySize);

	PacketHeader* pktHeader = reinterpret_cast<PacketHeader*>(StartPtr);
	pktHeader->PacketSize = memorySize;
	pktHeader->ProtocolID = ProtocolId;
	std::memcpy(&pktHeader[1], bufPtr, SerializedDataSize);

	return sendBuf;
}

SPtr_PacketSendBuf SendBuffersFactory::CreateFixSendPacketBuf(SendPktInfo::Fix pktDataType)
{
	SPtr_PacketSendBuf sendBuf{};

	return sendBuf;
}

SPtr_PacketSendBuf SendBuffersFactory::CreatePacket(const uint8_t* bufPtr, const uint16_t SerializedDataSize, uint16_t ProtocolId)
{

	/* [[PacketHeader][SendMemory]] */

	const SPtr_PacketSendBuf sendBuf = CreateVarSendPacketBuf(bufPtr, SerializedDataSize, ProtocolId, sizeof(PacketHeader) + SerializedDataSize);
	PacketHeader* pktheader = reinterpret_cast<PacketHeader*>(sendBuf->GetBuffer());


	//BYTE* dataPtr = static_cast<BYTE*>(sendBuf->GetBuffer()) + sizeof(PacketHeader);
	//std::memcpy(dataPtr, bufPtr, SerializedDataSize);
	//std::cout << pktheader->PacketSize << " " << pktheader->ProtocolID << std::endl;
	/* 1. PacketHeader 정보를 담는다. */
	

	//PacketHeader* SendBufPktHeader = reinterpret_cast<PacketHeader*>(sendBuf->GetBuffer());
	/* 문제발생! - mPtrFromMemPool 이게 왜 바뀌는거야 도대체 이 바뀌네?? */
	//SendBufPktHeader->PacketSize = sizeof(PacketHeader) + SerializedDataSize;
	//SendBufPktHeader->ProtocolID = ProtocolId;
	

	/* 2. PacketBuffer 정보를 담는다. */
	if (sendBuf == nullptr) {
		std::cout << "SendBufferFactory::CreatePacket - SendBuf = nullptr\n";
		return nullptr;
	}

	return sendBuf;
}

SPtr_SendPktBuf SendBuffersFactory::SPkt_Chat(UINT32 sessionID, std::string msg)
{
	flatbuffers::FlatBufferBuilder builder;

	auto msgOffset    = builder.CreateString(msg);
	auto ServerPacket = FBProtocol::CreateSPkt_Chat(builder, sessionID, msgOffset);

	builder.Finish(ServerPacket);

	const uint8_t* bufferPtr = builder.GetBufferPointer();
	const uint16_t serializedDataSize = static_cast<uint16_t>(builder.GetSize());

	return CreatePacket(bufferPtr, serializedDataSize, FBsProtocolID::SPkt_Chat);
}

