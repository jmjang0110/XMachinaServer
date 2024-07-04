#pragma once

class HeightMapImage
{
private:
	std::vector<float>	mHeightMapPixels{};

	int mWidth{};
	int mLength{};

public:
	HeightMapImage();

	// [fileName]�� ������ [width * height]��ŭ �о� �����Ѵ�.
	HeightMapImage(const std::string& fileName);

	virtual ~HeightMapImage() = default;

	float GetHeight(float x, float z) const;
	Vec3 GetHeightMapNormal(int x, int z) const;

	const std::vector<float>& GetHeightMapPixels() const { return mHeightMapPixels; }
	int GetHeightMapWidth() const { return mWidth; }
	int GetHeightMapLength() const { return mLength; }

public:

	void Init(const std::string& fileName);

};

