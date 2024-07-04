#include "pch.h"
#include "FileIO.h"
#include "HeightMapImage.h"

HeightMapImage::HeightMapImage()
{
}

HeightMapImage::HeightMapImage(const std::string& fileName)
{
	std::ifstream file = FileIO::OpenBinFile(fileName);

	FileIO::ReadVal(file, mWidth);
	FileIO::ReadVal(file, mLength);

	FileIO::ReadRange(file, mHeightMapPixels, mWidth * mLength);
}

float HeightMapImage::GetHeight(float x, float z) const
{
    return 0.0f;
}

Vec3 HeightMapImage::GetHeightMapNormal(int x, int z) const
{
    return Vec3();
}

void HeightMapImage::Init(const std::string& fileName)
{
	std::ifstream file = FileIO::OpenBinFile(fileName);

	FileIO::ReadVal(file, mWidth);
	FileIO::ReadVal(file, mLength);

	FileIO::ReadRange(file, mHeightMapPixels, mWidth * mLength);

}
