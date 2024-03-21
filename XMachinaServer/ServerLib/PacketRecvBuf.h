#pragma once

namespace RecvInfo {
	enum class Buf {
		Size = 0x10'000 // 64kb
	};
}

class PacketRecvBuf
{
private:
	std::vector<BYTE> mBuf = {};

public:
	PacketRecvBuf();
	~PacketRecvBuf();

};

