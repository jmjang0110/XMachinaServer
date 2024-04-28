#pragma once

/// +-------------------------------
///		  Server Framework
/// ________________________________
///		 서버 프레임워크 관리  
/// -------------------------------+


#define FRAMEWORK Framework::GetInst()
#define SEND_FACTORY Framework::GetInst()->GetSendFactory()
class Framework
{
	DECLARE_SINGLETON(Framework);

private:
	SPtr_ServerNetwork  mServer = {};
	SPtr_SendBufFactory mSendFactory = {};

public:
	Framework();
	~Framework();

public:
	bool Init(HINSTANCE& hInst);
	void Launch();

	SPtr_ServerNetwork GetServerNetwork() { return mServer; }
	SPtr_SendBufFactory GetSendFactory()  { return mSendFactory; }

};

