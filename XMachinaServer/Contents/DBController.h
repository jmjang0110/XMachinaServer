#pragma once

/// +-----------------------------------------------
///					   DB CONTROLLER 
/// ________________________________________________
/// GAME DATA BASE �� �����մϴ�.
/// --> [1] Game Data, [2] Connect With DataBase System 
/// 
/// ________________________________________________
///		 SQL Server Management Studio Express
/// -----------------------------------------------+

#include "DB_Player.h"
#include "DB_Monster.h"
#include "DB_NPC.h"
#include <sqlext.h>

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
	SQLHENV hEnv;
	SQLHDBC hDbc;

private:
	std::string mUserDSN_Name = "X_Machina_DB";
	std::string mUser;
	std::string mPassword;

private:
	X_Machina_DB mX_Machina_DB;

public:
	DBController();
	~DBController();

public:
	bool ConnectToDatabase(const char* dsn, const char* user, const char* password);
	void DisconnectFromDatabase();
	bool ExecuteQuery(const char* query);
	bool FetchData(const char* query);

	bool ReadDataFromDatabase(const char* query);
	bool WriteDataToDatabase(const char* query);

};

