#include "pch.h"
#include "ResourceManager.h"


DEFINE_SINGLETON(ResourceManager);


ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::Init()
{
	/// +----------------------------------------------------
	///	�� Height Map Image Init ( Terrain.bin )
	/// ----------------------------------------------------+
	mHeightMapImg = MEMORY->Make_Shared<HeightMapImage>();
	mHeightMapImg->Init("Contents/Resource/Terrain.bin");

}
