#pragma once


/// +-------------------------------
///		   ResourceManager
/// ________________________________
///	[ 설명 ] 
/// - 필요한 리소스를 로드한 후 참조해서 나눠쓴다.
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

