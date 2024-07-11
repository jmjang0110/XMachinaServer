#pragma once

#include "Component.h"


class Collider : public Component
{
private:

public:
	Collider();
	Collider(SPtr<GameObject> owner, ComponentInfo::Type Type);
	~Collider();

public:
public:
	virtual bool WakeUp()	override;
	virtual bool Start()	override;
	virtual bool Update()	override;


};

