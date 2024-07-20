#include "pch.h"
#include "Script_PheroDropper.h"

Script_PheroDropper::Script_PheroDropper()
{
}

Script_PheroDropper::Script_PheroDropper(SPtr<GameObject> owner, ScriptInfo::Type type)
	: Script(owner, type, static_cast<UINT32>(type))
{
}

Script_PheroDropper::~Script_PheroDropper()
{
}

SPtr<Component> Script_PheroDropper::Clone(SPtr<GameObject> copyOwner) const
{
	return SPtr<Component>();
}

void Script_PheroDropper::Activate()
{
}

void Script_PheroDropper::DeActivate()
{
}

bool Script_PheroDropper::WakeUp()
{

	return true;
}

bool Script_PheroDropper::Start()
{

	return true;
}

bool Script_PheroDropper::Update()
{

	return true;
}

void Script_PheroDropper::OnDestroy()
{
}
