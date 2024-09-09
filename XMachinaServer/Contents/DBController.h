#pragma once

/// +-----------------------------------------------
///					   DB CONTROLLER 
/// ________________________________________________
/// GAME DATA BASE 를 관리합니다.
/// --> [1] Game Data, [2] Connect With DataBase System 
/// 
/// ODBC : Open Database Connectivity 
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
	SQLHENV  mhEnv  = SQL_NULL_HANDLE;
	SQLHDBC  mhDbc  = SQL_NULL_HANDLE; /* Connection DB */
	SQLHSTMT mhStm  = SQL_NULL_HANDLE;


private:
	std::string mUserDSN_Name = "X_Machina_DB";
	std::string mUser;
	std::string mPassword;

private:
	X_Machina_DB mX_Machina_DB;

public:
	DBController();
	~DBController();

	void Init();

public:
	bool ConnectToDatabase(const wchar_t* dsn, const wchar_t* user, const wchar_t* password);
	void DisconnectFromDatabase();

	bool ExecuteQuery(const wchar_t* query);
	bool FetchData(const wchar_t* query);

	bool ReadDataFromDatabase(const wchar_t* query);
	bool WriteDataToDatabase(const wchar_t* query);


	void PrintSQLError(SQLSMALLINT handleType, SQLHANDLE handle);
	void CheckSQLReturn(SQLRETURN ret, SQLSMALLINT handleType, SQLHANDLE handle);

public:
	SQLHENV		SQLH_Env()		{ return mhEnv; }
	SQLHDBC		SQLH_Dbc()		{ return mhDbc; }
	SQLHSTMT	SQLH_Stmt()		{ return mhStm; }
};

