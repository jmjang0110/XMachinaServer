#pragma once

#include "GameObject.h"

class GameNPC : public GameObject
{
private:


public:
	GameNPC();
	GameNPC(UINT32 id); /* NPC 생성 아이디 - (생성되고 소멸될 때 까지 임시 아이디)*/
	virtual ~GameNPC();


public:
	virtual void Update() override;
	virtual void WakeUp() override;

	virtual void Activate();
	virtual void DeActivate();

	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;


};

