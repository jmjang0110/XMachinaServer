#pragma once

/// +-----------------------------------------------
///					   DB CONTROLLER 
/// ________________________________________________
/// GAME DATA BASE 를 관리합니다.
/// --> [1] Game Data, [2] Connect With DataBase System 
/// 
/// ________________________________________________
///		 SQL Server Management Studio Express
/// -----------------------------------------------+

#include "DB_Player.h"
#include "DB_Monster.h"
#include "DB_NPC.h"

/* X-Machina Game Data Base */
struct X_Machina_DB
{
	DB_Player*	PlayerDB  = nullptr;
	DB_Monster*	MonsterDB = nullptr;
	DB_NPC*		NPCDB     = nullptr;
};

#define DB_CONTROLLER DBController::GetInst()
class DBController
{
	DECLARE_SINGLETON(DBController);

private:
	X_Machina_DB mX_Machina_DB;

public:
	DBController();
	~DBController();

};

