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

void Script_PheroDropper::Clone(SPtr<Component> other) 
{
	Script::Clone(other);
	SPtr<Script_PheroDropper> otherScript = std::static_pointer_cast<Script_PheroDropper>(other);

}

void Script_PheroDropper::Activate()
{
	Script::Activate();

}

void Script_PheroDropper::DeActivate()
{
	Script::DeActivate();

}

bool Script_PheroDropper::WakeUp()
{
	Script::WakeUp();


	return true;
}

bool Script_PheroDropper::Start()
{
	Script::Start();


	return true;
}

bool Script_PheroDropper::Update()
{
	Script::Update();

	return true;
}

void Script_PheroDropper::OnDestroy()
{
	Script::OnDestroy();

}
