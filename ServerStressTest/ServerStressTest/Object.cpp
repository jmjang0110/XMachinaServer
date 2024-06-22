#include "pch.h"
#include "Object.h"
#include "Win32RenderManager.h"


Object::Object(std::string name)
	: Entity(name)
{
}

Object::Object()
	:Entity("Default")
{
}

Object::~Object()
{
	for (Script* scr : mScripts) {
		SAFE_DELETE(scr);
	}
	mScripts.clear();

}

void Object::SetImage(std::wstring path, VECTOR2 pixelLT, VECTOR2 pixelsize)
{
	if (mImage.image) {
		mImage.image.Destroy();
	}

	mImage.image.Load(path.c_str());
	mImage.LT = pixelLT;
	mImage.Size = pixelsize;// -mImage.LT;

	if (mImage.image) {
		mImage.TotalSize.x = mImage.image.GetWidth();
		mImage.TotalSize.y = mImage.image.GetHeight();
	}


}

void Object::SetImageLT(VECTOR2 lt)
{
	mImage.LT = lt;
}

void Object::SetImageSize(VECTOR2 size)
{
	mImage.Size = size;
}

void Object::SetScale(VECTOR2 scale)
{
	mTransform.Scale = scale;
}

void Object::setPos(VECTOR2 pos)
{
	/*
			 Window
	_____________________________
	|   Pos ( Pixel )			|
	|	бщ						|
	|	------					|
	|	|	 |					|
	|	------					|
	|							|
	|							|
	|							|
	_____________________________
	*/

	mTransform.Position = pos;
}

void Object::PushBack(Script* script)
{
	mScripts.push_back(script);
}


void Object::Init()
{
}

void Object::Update()
{

}

void Object::Render()
{
	if (bActive == false)
		return;

	if (mImage.image) {
		RENDER_MGR->DrawImage(mImage, mTransform.Position, mTransform.Scale);
	}

}
