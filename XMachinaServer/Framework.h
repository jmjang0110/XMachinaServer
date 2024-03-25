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


public:
	Framework();
	~Framework();

public:
	bool Init(HINSTANCE& hInst);
	void Launch();

	SPtr_ServerNetwork GetServerNetwork() { return mServer; }

};

