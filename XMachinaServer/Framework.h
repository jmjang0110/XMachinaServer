#pragma once

/// +-------------------------------
///		  Server Framework
/// ________________________________
///		 서버 프레임워크 관리  
/// -------------------------------+


#define FRAMEWORK Framework::GetInst()
class Framework
{
	DECLARE_SINGLETON(Framework);

private:
	SPtr_ServerNetwork mServer = {};


public:
	Framework();
	~Framework();

public:
	bool Init(HINSTANCE& hInst);
	void Launch();

};

