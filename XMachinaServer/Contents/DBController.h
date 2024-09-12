#pragma once

/// +-----------------------------------------------
///					   DB CONTROLLER 
/// ________________________________________________
/// GAME DATA BASE �� �����մϴ�.
/// --> [1] Game Data, [2] Connect With DataBase System 
/// 
/// ODBC : Open Database Connectivity 
/// ________________________________________________
///		 SQL Server Management Studio Express
/// -----------------------------------------------+

#include "DB_Object.h"

enum class DataBaseEventType {
	None,
	Query,
	_count,
};

// ���� ���� �켱���� 
enum class QueryPriority {
	None,

	_count,
};
struct DataBaseEvent {
	QueryPriority	  QPriority     = QueryPriority::None;
	DataBaseEventType DBEventType   = DataBaseEventType::None;
	SPtr<DB_Object>   DBObject      = nullptr;
	std::wstring	  Query         = {};

	bool operator< (const DataBaseEvent& rhs) const {
		return static_cast<int>(this->QPriority) > static_cast<int>(rhs.QPriority); // ���� ������ QueryPriority�� ���� �켱������ �������� ����
	}
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
	Concurrency::concurrent_priority_queue<DataBaseEvent> mDBEventPQ;
	bool mLaunchThread = true;

public:
	DBController();
	~DBController();

	void Init();

public:
	void Launch();
	void Process_DataBaseEvent(DataBaseEvent ev);
	void PushDataBaseEvent(DataBaseEvent ev);
	void PushDataBaseEvent(QueryPriority quryPriority, DataBaseEventType evType, SPtr<DB_Object> dbObj, const wchar_t* query);
	void ExecuteAllDataBaseEvents();

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

