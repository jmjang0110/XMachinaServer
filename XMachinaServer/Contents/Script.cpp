#include "pch.h"
#include "Script.h"

Script::Script()
	: Component()
{
}

Script::Script(SPtr<GameObject> owner, ScriptInfo::Type type, UINT32 id)
	: Component(owner, ComponentInfo::Type::Script, id)
{
	mType = type;
}

Script::~Script()
{
}

void Script::Activate()
{
	Component::Activate();

	OnEnable();

}

void Script::DeActivate()
{
	Component::DeActivate();

	OnDisable();
}

void Script::OnEnable()
{

}

void Script::OnDisable()
{

}

bool Script::WakeUp()
{
	return false;
}

bool Script::Start()
{
	return false;
}

bool Script::Update()
{
	return false;
}

void Script::OnDestroy()
{
}
