#include "pch.h"
#include "Script_MissileLauncher.h"

Script_MissileLauncher::Script_MissileLauncher()
	: Script_Bullet()

{
}

Script_MissileLauncher::Script_MissileLauncher(SPtr<GameObject> owner, ScriptInfo::Type type)
	: Script_Bullet(owner, type)
{

}

Script_MissileLauncher::~Script_MissileLauncher()
{
}
