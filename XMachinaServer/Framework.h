#pragma once

/// +-------------------------------
///		  Server Framework
/// ________________________________
///		 서버 프레임워크 관리  
/// -------------------------------+

#define SERVER_NETWORK Framework::GetInst()->GetServerNetwork()
#define FRAMEWORK Framework::GetInst()
#define SEND_FACTORY Framework::GetInst()->GetSendFactory()
class Framework
{
	DECLARE_SINGLETON(Framework);

private:
	std::wstring		mServerIP    = L"127.0.0.1";
	SPtr_ServerNetwork  mServer      = {};
	SPtr_SendBufFactory mSendFactory = {};
	
public:
	Framework();
	~Framework();

public:
	bool Init(HINSTANCE& hInst);
	void Launch();

	SPtr_ServerNetwork GetServerNetwork() { return mServer; }
	SPtr_SendBufFactory GetSendFactory()  { return mSendFactory; }
	long long GetCurrentTimeMilliseconds();
	std::string GetLocalIPv4Address();
	
	std::wstring string_to_wstring(const std::string& str);
	
};

