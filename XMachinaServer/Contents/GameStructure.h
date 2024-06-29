#pragma once

#include "GameObject.h"

class GameStructure : public GameObject
{
private:


public:
	GameStructure();
	GameStructure(UINT32 sessionID, SPtr_GameSession owner);
	~GameStructure();

public:
	virtual void Update() override;
	virtual void WakeUp() override;
	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;


};

