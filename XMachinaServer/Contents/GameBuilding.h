#pragma once

#include "GameObject.h"

class GameBuilding : public GameObject
{
private:


public:
	GameBuilding();
	GameBuilding(UINT32 sessionID, SPtr_GameSession owner);
	~GameBuilding();

public:
	virtual void Update() override;
	virtual void WakeUp() override;
	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;


};

