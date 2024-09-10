#include "pch.h"
#include "DB_EnemyStat.h"

DB_EnemyStat::DB_EnemyStat()
	: DB_Object()
{
}

DB_EnemyStat::~DB_EnemyStat()
{
}

void DB_EnemyStat::FetchDataFromDataBase(const wchar_t* query)
{    
    // Call the base class method to prepare the statement handle
    DB_Object::FetchDataFromDataBase(query);

    // Fetch the data
    while ((mSQL_Ret = SQLFetch(mSQL_hStmt)) == SQL_SUCCESS || mSQL_Ret == SQL_SUCCESS_WITH_INFO) {
        SQLGetData(mSQL_hStmt, 1, SQL_C_CHAR, &PK_Name[0], 50, nullptr);
        SQLGetData(mSQL_hStmt, 2, SQL_C_SLONG, &Level, 0, nullptr);
        SQLGetData(mSQL_hStmt, 3, SQL_C_SLONG, &PheroLevel, 0, nullptr);
        SQLGetData(mSQL_hStmt, 4, SQL_C_FLOAT, &MoveSpeed, 0, nullptr);
        SQLGetData(mSQL_hStmt, 5, SQL_C_FLOAT, &RotationSpeed, 0, nullptr);
        SQLGetData(mSQL_hStmt, 6, SQL_C_FLOAT, &AttackRotationSpeed, 0, nullptr);
        SQLGetData(mSQL_hStmt, 7, SQL_C_FLOAT, &DetectionRange, 0, nullptr);
        SQLGetData(mSQL_hStmt, 8, SQL_C_FLOAT, &AttackRate, 0, nullptr);
        SQLGetData(mSQL_hStmt, 9, SQL_C_FLOAT, &AttackRange, 0, nullptr);
        SQLGetData(mSQL_hStmt, 10, SQL_C_FLOAT, &AttackCoolTime, 0, nullptr);
        SQLGetData(mSQL_hStmt, 11, SQL_C_CHAR, &Attack1AnimName[0], 50, nullptr);
        SQLGetData(mSQL_hStmt, 12, SQL_C_CHAR, &Attack2AnimName[0], 50, nullptr);
        SQLGetData(mSQL_hStmt, 13, SQL_C_CHAR, &Attack3AnimName[0], 50, nullptr);
        SQLGetData(mSQL_hStmt, 14, SQL_C_CHAR, &GetHitAnimName[0],  50, nullptr);
        SQLGetData(mSQL_hStmt, 15, SQL_C_CHAR, &DeathAnimName[0],   50, nullptr);
    }

    // Clean up
    ::SQLFreeHandle(SQL_HANDLE_STMT, mSQL_hStmt);
}

void DB_EnemyStat::LoadFromDataBase(std::string PK_Name)
{ 
    // Construct the query
    wchar_t query[1024];
    swprintf(query, 1024, L"SELECT Name, Level, PheroLevel, MoveSpeed, RotationSpeed, AttackRotationSpeed, DetectionRange, AttackRate, AttackRange, AttackCoolTime, Attack1AnimName, Attack2AnimName, Attack3AnimName, GetHitAnimName, DeathAnimName FROM EnemyStat WHERE Name = '%S'", PK_Name.c_str());

    // Fetch data from the database
    FetchDataFromDataBase(query);

    // Output fetched data for verification
    std::wcout << L"Name: " << PK_Name.c_str() << L", Level: " << Level
        << L", PheroLevel: " << PheroLevel
        << L", MoveSpeed: " << MoveSpeed
        << L", RotationSpeed: " << RotationSpeed
        << L", AttackRotationSpeed: " << AttackRotationSpeed
        << L", DetectionRange: " << DetectionRange
        << L", AttackRate: " << AttackRate
        << L", AttackRange: " << AttackRange << " \n"
        << L", AttackCoolTime: " << AttackCoolTime
        << L", Attack1AnimName: " << Attack1AnimName
        << L", Attack2AnimName: " << Attack2AnimName
        << L", Attack3AnimName: " << Attack3AnimName
        << L", GetHitAnimName: " << GetHitAnimName
        << L", DeathAnimName: " << DeathAnimName<< "\n\n";
}
