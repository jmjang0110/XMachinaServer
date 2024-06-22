#pragma once

enum class OP_TYPE {
	SEND,
	RECV,
	DO_MOVE,

};

struct OverlappedEx
{
	WSAOVERLAPPED over;

	WSABUF			WsaBuf;
	unsigned char	IOCP_Buf[MAX_BUFF_SIZE];
	OP_TYPE			Event_Type;
	int				Event_Target;

};

