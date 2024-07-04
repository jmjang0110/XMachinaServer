#pragma once

#include "Component.h"


class Collider : public Component
{
private:

public:
	Collider();
	Collider(UINT32 id);
	~Collider();

public:
public:
	virtual bool WakeUp()	override;
	virtual bool Start()	override;
	virtual bool Update()	override;


};

