#pragma once
#include "GameMonster.h"
#include "TimeManager.h"



class Monster_Onyscidus : public GameMonster
{
private:

public:
	Monster_Onyscidus();
	Monster_Onyscidus(UINT32 sessionID, Coordinate sectorIdx);
	~Monster_Onyscidus();

public:
	void Init();

public:
	virtual void Update() override;
	virtual void WakeUp() override;

	virtual void Activate();
	virtual void DeActivate();

	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;



};

