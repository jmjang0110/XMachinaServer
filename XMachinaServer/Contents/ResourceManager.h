#pragma once


/// +-------------------------------
///		   ResourceManager
/// ________________________________
///	[ ���� ] 
/// - �ʿ��� ���ҽ��� �ε��� �� �����ؼ� ��������.
/// ________________________________
/// 
/// 
/// -------------------------------+


#define RESOURCE_MGR ResourceManager::GetInst()

class ResourceManager
{
	DECLARE_SINGLETON(ResourceManager);

private:


public:
	ResourceManager();
	~ResourceManager();

};

