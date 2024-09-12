#include "pch.h"
#include "DB_PheroDropInfo.h"
#include "DBController.h"

DB_PheroDropInfo::DB_PheroDropInfo()
	: DB_Object()
{
}

DB_PheroDropInfo::~DB_PheroDropInfo()
{
}

void DB_PheroDropInfo::FetchDataFromDataBase(const wchar_t* query)
{   
    // Call the base class method to prepare the statement handle
    DB_Object::FetchDataFromDataBase(query);

    // Fetch the data
    while ((mSQL_Ret = SQLFetch(mSQL_hStmt)) == SQL_SUCCESS || mSQL_Ret == SQL_SUCCESS_WITH_INFO) {
        SQLGetData(mSQL_hStmt, 1, SQL_C_SLONG, &PK_Level, 0, nullptr);
        SQLGetData(mSQL_hStmt, 2, SQL_C_SLONG, &MinPheroDropCount, 0, nullptr);
        SQLGetData(mSQL_hStmt, 3, SQL_C_SLONG, &MaxPheroDropCount, 0, nullptr);
        SQLGetData(mSQL_hStmt, 4, SQL_C_FLOAT, &L1PheroDropRate, 0, nullptr);
        SQLGetData(mSQL_hStmt, 5, SQL_C_FLOAT, &L2PheroDropRate, 0, nullptr);
        SQLGetData(mSQL_hStmt, 6, SQL_C_FLOAT, &L3PheroDropRate, 0, nullptr);
    }

    // Clean up
    ::SQLFreeHandle(SQL_HANDLE_STMT, mSQL_hStmt);

    // Output the data
    std::wcout << L"Level: " << PK_Level
        << L", MinPheroDropCount: " << MinPheroDropCount
        << L", MaxPheroDropCount: " << MaxPheroDropCount
        << L", L1PheroDropRate: " << L1PheroDropRate
        << L", L2PheroDropRate: " << L2PheroDropRate
        << L", L3PheroDropRate: " << L3PheroDropRate
        << std::endl;
}

void DB_PheroDropInfo::LoadFromDataBase(int level)
{    
    // Construct the query
    wchar_t query[256];
    swprintf(query, 256, L"SELECT Level, MinPheroDropCount, MaxPheroDropCount, L1PheroDropRate, L2PheroDropRate, L3PheroDropRate FROM PheroDropInfo WHERE Level = %d", level);

    DB_CONTROLLER->PushDataBaseEvent(QueryPriority::None, DataBaseEventType::Query, shared_from_this(), query);
}
