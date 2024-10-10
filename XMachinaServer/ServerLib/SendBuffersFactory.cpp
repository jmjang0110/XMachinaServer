#include "pch.h"
#include "SendBuffersFactory.h"
#include "ServerMemoryPool.h"
#include "MemoryManager.h"
#include "../Framework.h"
#include "Contents/GameObject.h"


#undef max
#include <flatbuffers/flatbuffers.h>
#include "Protocol/FBProtocol_generated.h"
#include "Protocol/Enum_generated.h"
#include "Protocol/Struct_generated.h"
#include "Protocol/Transform_generated.h"


SendBuffersFactory::SendBuffersFactory()
{

}

SendBuffersFactory::~SendBuffersFactory()
{
	for (auto& pair : mMemPools_VarPkt) {
		pair.second = nullptr;
	}
	for (auto& pair : mMemPools_FixPkt) {
		pair.second = nullptr;
	}
	
	mMemPools_VarPkt.clear();
	mMemPools_FixPkt.clear();

	mMemPools_SptrSendPkt = nullptr;

}

void SendBuffersFactory::InitPacketMemoryPools()
{
	mMemPools_SptrSendPkt = MEMORY->Make_Shared<ServerMemoryPool>();
	mMemPools_SptrSendPkt->Init(sizeof(PacketSendBuf), 100);

/// +-------------------------
///	  Variable Length Buffer
/// -------------------------+
	AddMemoryPool(SendPktInfo::Var::BYTES_32, 10'0);
	AddMemoryPool(SendPktInfo::Var::BYTES_64, 10'0);
	AddMemoryPool(SendPktInfo::Var::BYTES_128, 10'0);
	AddMemoryPool(SendPktInfo::Var::BYTES_256, 1000'000);
	AddMemoryPool(SendPktInfo::Var::BYTES_512, 50000);
	AddMemoryPool(SendPktInfo::Var::BYTES_1024, 50000);
	AddMemoryPool(SendPktInfo::Var::BYTES_2048, 50000);
	AddMemoryPool(SendPktInfo::Var::BYTES_4096, 5000);
}

void* SendBuffersFactory::Pull_SendPkt()
{
	mPullCount.fetch_add(1);
	void* ptr = mMemPools_SptrSendPkt->Pull();
	return ptr;

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
	else if (512 < memorySize && memorySize <= 1024) {
		return mMemPools_VarPkt[SendPktInfo::Var::BYTES_1024]->Pull();
	}
	else if (1024 < memorySize && memorySize <= 2048) {
		return mMemPools_VarPkt[SendPktInfo::Var::BYTES_2048]->Pull();
	}
	else if (2048 < memorySize && memorySize <= 4096) {
		return mMemPools_VarPkt[SendPktInfo::Var::BYTES_4096]->Pull();
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
	else if (512 < memorySize && memorySize <= 1024) {
		mMemPools_VarPkt[SendPktInfo::Var::BYTES_1024]->Push(ptr);
	}
	else if (1024 < memorySize && memorySize <= 2048) {
		mMemPools_VarPkt[SendPktInfo::Var::BYTES_2048]->Push(ptr);
	}
	else if (2048 < memorySize && memorySize <= 4096) {
		mMemPools_VarPkt[SendPktInfo::Var::BYTES_4096]->Push(ptr);
	}
}

void SendBuffersFactory::Push_FixPkt(SendPktInfo::Fix type, void* ptr)
{
	mMemPools_FixPkt[type]->Push(ptr);
}

void SendBuffersFactory::Push_SendPkt(void* ptr)
{
	mPushCount.fetch_add(1);
	mMemPools_SptrSendPkt->Push(ptr);
}

SPtr_PacketSendBuf SendBuffersFactory::CreateVarSendPacketBuf(const uint8_t* bufPtr, const uint16_t SerializedDataSize, uint16_t ProtocolId, size_t memorySize)
{

	/* 메모리 풀에서 메모리를 받아와서 shared_ptr 로 SendPacket Buffer 를 만들어 반환하자! */
	
	size_t offsetMemSize = 0;
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
	else if (memorySize <= 1024) {
		offsetMemSize = 1024 - memorySize;
	}
	else if (memorySize <= 2048) {
		offsetMemSize = 2048 - memorySize;
	}
	else if (memorySize <= 4096) {
		offsetMemSize = 4096 - memorySize;
	}
	if (offsetMemSize < 0)
		return nullptr;

 	BYTE* ptr                  = reinterpret_cast<BYTE*>(Pull_VarPkt(memorySize));
	BYTE* StartPtr             = ptr + offsetMemSize;


	void*				dstPtr      = Pull_SendPkt();
	SPtr_PacketSendBuf	sendBuf     = Make_Shared(dstPtr, ptr, static_cast<UINT16>(memorySize + offsetMemSize), StartPtr, static_cast<UINT32>(memorySize));
	PacketHeader*		pktHeader   = reinterpret_cast<PacketHeader*>(StartPtr);

	pktHeader->PacketSize           = static_cast<UINT16>(memorySize);
	pktHeader->ProtocolID           = ProtocolId;
	if (ProtocolId == 0)
		assert(0);

	std::memcpy(&pktHeader[1], bufPtr, SerializedDataSize);

	return sendBuf;
}

SPtr_PacketSendBuf SendBuffersFactory::CreateFixSendPacketBuf(SendPktInfo::Fix pktDataType)
{
	SPtr_PacketSendBuf sendBuf{};

	return sendBuf;
}

void SendBuffersFactory::AddMemoryPool(SendPktInfo::Var memoryBlockSize, int cnt)
{
	if (mMemPools_VarPkt.find(memoryBlockSize) != mMemPools_VarPkt.end())
		return;

	mMemPools_VarPkt[memoryBlockSize] = MEMORY->Make_Shared<ServerMemoryPool>();
	mMemPools_VarPkt[memoryBlockSize]->Init(static_cast<size_t>(memoryBlockSize), cnt);
}

SPtr_PacketSendBuf SendBuffersFactory::CreatePacket(const uint8_t* bufPtr, const uint16_t SerializedDataSize, uint16_t ProtocolId)
{
	/* [[PacketHeader][SendMemory]] */
	const SPtr_PacketSendBuf sendBuf = CreateVarSendPacketBuf(bufPtr, SerializedDataSize, ProtocolId, sizeof(PacketHeader) + SerializedDataSize);
	PacketHeader* pktheader = reinterpret_cast<PacketHeader*>(sendBuf->GetBuffer());
	if (sendBuf == nullptr) {
		std::cout << "SendBufferFactory::CreatePacket - SendBuf = nullptr\n";
		return nullptr;
	}
	return sendBuf;
}


