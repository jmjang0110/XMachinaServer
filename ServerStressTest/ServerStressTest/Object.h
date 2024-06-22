#pragma once
#include "Entity.h"

class Script;

/// +----------------
///		  IMAGE
/// ----------------+
struct Image
{
	ATL::CImage  image = {};
	VECTOR2		 LT = {};
	VECTOR2		 Size = {};

	VECTOR2		 TotalSize = {};
};

/// +----------------
///		TRANSFORM
/// ----------------+
struct Transform
{
	VECTOR2  Position = {};
	VECTOR2	 Scale = {};
	VECTOR2	 Rotation = {};

	VECTOR2	 MoveDir = {};
};


/// +----------------
///		 OBJECT
/// ----------------+
class Object : public Entity
{
private:
	Image		mImage{};
	Transform	mTransform{};

	std::vector<Script*> mScripts{};

	bool bActive = true;

public:
	Object(std::string name);
	Object();
	virtual ~Object();

	Transform& GetTransform() { return mTransform; }
	Image& GetImage() { return mImage; }

	void		SetImage(std::wstring path, VECTOR2 pixelLT, VECTOR2 pixelsize);
	void		SetImageLT(VECTOR2 lt);
	void		SetImageSize(VECTOR2 size);

	void SetScale(VECTOR2 scale);
	void setPos(VECTOR2 pos);
	void PushBack(Script* script);


public:
	void Init();
	void Update();
	void Render();

public:
	void Activate() { bActive = true; }
	void DeActivate() { bActive = false; }

};

