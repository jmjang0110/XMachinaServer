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

public:
	Framework();
	~Framework();

public:
	bool Init(HINSTANCE& hInst);
	void Launch();

};

