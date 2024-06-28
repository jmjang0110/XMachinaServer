#pragma once
#include "GameMonster.h"
#include "TimeManager.h"



class Monster_Onyscidus : public GameMonster
{
private:
	float speed = 0.f;


public:
	virtual void Update() override;
	virtual void WakeUp() override;
	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;

public:
	Monster_Onyscidus();
	Monster_Onyscidus(UINT32 sessionID);
	~Monster_Onyscidus();

};

