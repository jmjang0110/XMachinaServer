#include "pch.h"
#include "DB_UserInfo.h"
#include "DBController.h"
#include "GameSession.h"
#include "FBsPacketFactory.h"

DB_UserInfo::DB_UserInfo()
    : DB_Object()

{
}

DB_UserInfo::~DB_UserInfo()
{
}

void DB_UserInfo::FetchDataFromDataBase(const wchar_t* query)
{    
    // Call the base class method to prepare the statement handle
    DB_Object::FetchDataFromDataBase(query);

    // Variables to store fetched data
    char ID[51]       = { 0 };    // VARCHAR(50) so 51 chars to include null terminator
    char Password[51] = { 0 };    // VARCHAR(50) so 51 chars to include null terminator

    // Fetch the data
    while ((mSQL_Ret = SQLFetch(mSQL_hStmt)) == SQL_SUCCESS || mSQL_Ret == SQL_SUCCESS_WITH_INFO) {
        SQLGetData(mSQL_hStmt, 1, SQL_C_CHAR, ID, sizeof(ID), nullptr);  // Fetch the ID
        SQLGetData(mSQL_hStmt, 2, SQL_C_CHAR, Password, sizeof(Password), nullptr); // Fetch the Password
    }

    ::SQLFreeHandle(SQL_HANDLE_STMT, mSQL_hStmt);

    // Print the fetched results
    std::wcout << L"ID: " << ID << L", Password: " << Password << std::endl;
    bool IsSuccess = IsLoginSuccessful(ID, Password);

    /* Send Log In Packet */
    auto spkt = FBS_FACTORY->SPkt_LogIn(IsSuccess); // Fail Or Success 
    mOwnerSession->Send(spkt);

}

void DB_UserInfo::LoadFromDataBase(std::string PK_ID, std::string inputPassword)
{
    ID       = PK_ID;
    Password = inputPassword;

    // Construct the query
    wchar_t query[256];
    swprintf(query, 256, L"SELECT ID, Password FROM UserInfo WHERE ID = '%S'", PK_ID.c_str());

    // Push the database query event
    DB_CONTROLLER->PushDataBaseEvent(QueryPriority::None, DataBaseEventType::Query, shared_from_this(), query);
}

bool DB_UserInfo::IsLoginSuccessful(const std::string& inputID, const std::string& inputPassword)
{
    // Check if the fetched ID and Password match the provided ones
    if (ID == inputID && Password == inputPassword) {
        IsLogInSuccess = true;
        return true; // Login successful
    }
    else {
        IsLogInSuccess = false;
        return false; // Login failed
    }
}