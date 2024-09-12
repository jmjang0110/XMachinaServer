#include "pch.h"
#include "DB_Phero.h"
#include "DBController.h"

DB_Phero::DB_Phero()
	: DB_Object()
{
}

DB_Phero::~DB_Phero()
{
}

void DB_Phero::FetchDataFromDataBase(const wchar_t* query)
{
    // Call the base class method to prepare the statement handle
    DB_Object::FetchDataFromDataBase(query);

    // Fetch the data
    while ((mSQL_Ret = SQLFetch(mSQL_hStmt)) == SQL_SUCCESS || mSQL_Ret == SQL_SUCCESS_WITH_INFO) {
        SQLGetData(mSQL_hStmt, 1, SQL_C_SLONG, &PK_Level, 0, nullptr);
        SQLGetData(mSQL_hStmt, 2, SQL_C_FLOAT, &LifeTime_second, 0, nullptr);
        SQLGetData(mSQL_hStmt, 3, SQL_C_FLOAT, &Amount, 0, nullptr);
    }

    // Clean up
    ::SQLFreeHandle(SQL_HANDLE_STMT, mSQL_hStmt);

    // Now you can use phero.Level, phero.LifeTime_second, phero.Amount
    std::wcout << L"Level: " << PK_Level << L", LifeTime_second: " << LifeTime_second << L", Amount: " << Amount << std::endl;

}

void DB_Phero::LoadFromDataBase(int Level)
{
    // Construct the query
    wchar_t query[256];
    swprintf(query, 256, L"SELECT Level, LifeTime_second, Amount FROM Phero WHERE Level = %d", Level);

    DB_CONTROLLER->PushDataBaseEvent(QueryPriority::None, DataBaseEventType::Query, shared_from_this(), query);
}