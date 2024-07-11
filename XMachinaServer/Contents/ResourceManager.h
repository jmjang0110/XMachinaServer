#pragma once

#include "HeightMapImage.h"

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
	SPtr<HeightMapImage> mHeightMapImg;


public:
	ResourceManager();
	~ResourceManager();

public:
	void Init();

public:
	/// +--------------------------------------------------
	///	�� Getter
	/// --------------------------------------------------+
	SPtr<HeightMapImage> GetHeightMapImage() { return mHeightMapImg; }

};

