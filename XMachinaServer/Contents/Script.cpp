#include "pch.h"
#include "Script.h"

Script::Script()
	: Component()
{
}

Script::Script(ScriptInfo::Type type, UINT32 id)
	: Component(id, ComponentInfo::Type::Script)
{
	mType = type;
}

Script::~Script()
{
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
