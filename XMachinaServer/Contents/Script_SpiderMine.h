#pragma once
#include "Script_ExplosiveBullet.h"

class GameObject;
class Script_SpiderMine : public Script_ExplosiveBullet
{
private:
	float mMass           = {};
	float mDrag           = {};
	float mRotationSpeed  = {};
	float mRotationX      = {};
	float mRotationXSpeed = {};
	float mPlantY         = {};
	bool  mIsPlanted      = {};


public:
	Script_SpiderMine(); 
	Script_SpiderMine(SPtr<GameObject> owner);
	virtual ~Script_SpiderMine();

public:
	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);

	virtual void Start() override;
	virtual void Update() override;

	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0);
};

