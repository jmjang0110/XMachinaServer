#pragma once

#include "GameObject.h"

class GameBullet : public GameObject	
{

private:

public:
	GameBullet();
	GameBullet(UINT32 sessionID, SPtr_GameSession owner);
	~GameBullet();

public:
	virtual void Update() override;
	virtual void WakeUp() override;
	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;


};


