#pragma once

/// +-------------------------------
///		  Server Framework
/// ________________________________
///		 ���� �����ӿ�ũ ����  
/// -------------------------------+


#define FRAMEWORK Framework::GetInst()
class Framework
{
	DECLARE_SINGLETON(Framework);

private:
	SPtr_ServerNetwork mServer = {};
	SPtr_SendBufFactory mSendFactory = {};

public:
	Framework();
	~Framework();

public:
	bool Init(HINSTANCE& hInst);
	void Launch();

	SPtr_ServerNetwork GetServerNetwork() { return mServer; }
	SPtr_SendBufFactory GetSendFactory() { return mSendFactory; }

};

