#pragma once

enum class OP_TYPE {
	SEND,
	RECV,
	DO_MOVE,

};

class OverlappedEx
{
	WSAOVERLAPPED over;

	WSABUF wsabuf;
	unsigned char IOCP_Buf[MAX_BUFF_SIZE];



};

