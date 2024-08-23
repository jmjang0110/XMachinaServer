#pragma once
#include "Script_Bullet.h"

class GameObject;
class Script_MissileLauncher : public Script_Bullet
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
	Script_MissileLauncher(); 
	Script_MissileLauncher(SPtr<GameObject> owner);
	virtual ~Script_MissileLauncher();

public:

	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);

};

