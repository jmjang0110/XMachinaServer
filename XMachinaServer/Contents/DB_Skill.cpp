#include "pch.h"
#include "DB_Skill.h"
#include "DBController.h"

DB_Skill::DB_Skill()
    : DB_Object()
{
}

DB_Skill::~DB_Skill()
{
}

void DB_Skill::FetchDataFromDataBase(const wchar_t* query)
{
    // Call the base class method to prepare the statement handle
    DB_Object::FetchDataFromDataBase(query);

    char input_Name[51] = { 0 };

    // Fetch the data
    while ((mSQL_Ret = SQLFetch(mSQL_hStmt)) == SQL_SUCCESS || mSQL_Ret == SQL_SUCCESS_WITH_INFO) {
        SQLGetData(mSQL_hStmt, 1, SQL_C_CHAR, &input_Name[0], 51, nullptr);
        SQLGetData(mSQL_hStmt, 2, SQL_C_FLOAT, &CoolTime, 0, nullptr);
        SQLGetData(mSQL_hStmt, 3, SQL_C_FLOAT, &ActiveDurationTime, 0, nullptr);
    }

    Name = input_Name;
    // Clean up
    ::SQLFreeHandle(SQL_HANDLE_STMT, mSQL_hStmt);

    // Output fetched data for verification
    std::cout <<"Name: " << Name
        << ", CoolTime: " << CoolTime
        << ", ActiveDurationTime: " << ActiveDurationTime
        << "\n\n";
}

void DB_Skill::LoadFromDataBase(std::string PK_Name)
{
    // Construct the query to fetch data for the skill with the given Name (primary key)
    wchar_t query[1024];
    swprintf(query, 1024, L"SELECT Name, CoolTime, ActiveDurationTime FROM SkillInfo WHERE Name = '%S'", PK_Name.c_str());

    // Push the query to the database controller for execution
    DB_CONTROLLER->PushDataBaseEvent(QueryPriority::None, DataBaseEventType::Query, shared_from_this(), query);
}
