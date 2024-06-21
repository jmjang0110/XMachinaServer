#include "pch.h"
#include "DBController.h"
#include "ServerLib/MemoryManager.h"

DEFINE_SINGLETON(DBController);

DBController::DBController()
{

}

DBController::~DBController()
{
	SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

}

void DBController::Init()
{
	mX_Machina_DB.MonsterDB = MEMORY->New<DB_Monster>();
	mX_Machina_DB.NPCDB     = MEMORY->New<DB_NPC>();
	mX_Machina_DB.PlayerDB  = MEMORY->New<DB_Player>();

}

bool DBController::ConnectToDatabase(const wchar_t* dsn, const wchar_t* user, const wchar_t* password) {
	SQLRETURN ret;

	// 환경 핸들 할당
	ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		LOG_MGR->Cout("환경 핸들 할당 실패\n");
		return false;
	}

	// ODBC 버전 설정
	ret = ::SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), 0);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		LOG_MGR->Cout("ODBC 버전 설정 실패\n");
		::SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
		return false;
	}

	// 연결 핸들 할당
	ret = ::SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		LOG_MGR->Cout("연결 핸들 할당 실패\n");
		::SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
		return false;
	}

	// 데이터베이스 연결
	ret = ::SQLConnect(hDbc, (SQLWCHAR*)dsn, SQL_NTS, (SQLWCHAR*)user, SQL_NTS, (SQLWCHAR*)password, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		CheckSQLReturn(ret, SQL_HANDLE_DBC, hDbc);
		LOG_MGR->Cout("데이터베이스 연결 실패\n");
		::SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
		::SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
		return false;
	}

	std::cout << "데이터베이스 연결 성공" << std::endl;
	return true;
}

void DBController::DisconnectFromDatabase() {
	if (hDbc != SQL_NULL_HDBC) {
		::SQLDisconnect(hDbc);
		::SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
	}
	if (hEnv != SQL_NULL_HENV) {
		::SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
	}
}

bool DBController::ExecuteQuery(const wchar_t* query) {
	SQLHSTMT hStmt;
	SQLRETURN ret;

	// 문장 핸들 할당
	ret = ::SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		LOG_MGR->Cout("문장 핸들 할당 실패\n");
		return false;
	}

	// 쿼리 실행
	ret = ::SQLExecDirect(hStmt, (SQLWCHAR*)query, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		LOG_MGR->Cout("쿼리 실행 실패\n");
		::SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		return false;
	}

	::SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	return true;
}

bool DBController::FetchData(const wchar_t* query) {
	SQLHSTMT	hStmt     = {};
	SQLRETURN	ret       = {};
	SQLCHAR		col1[256] = {};
	SQLCHAR		col2[256] = {};

	// 문장 핸들 할당
	ret = ::SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		LOG_MGR->Cout("환경 핸들 할당 실패\n");
		return false;
	}

	// 쿼리 실행
	ret = SQLExecDirect(hStmt, (SQLWCHAR*)query, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		LOG_MGR->Cout("쿼리 실행 실패\n");
		::SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		return false;
	}

	// 데이터 페칭
	while (SQLFetch(hStmt) == SQL_SUCCESS) {
		::SQLGetData(hStmt, 1, SQL_C_CHAR, col1, sizeof(col1), NULL);
		::SQLGetData(hStmt, 2, SQL_C_CHAR, col2, sizeof(col2), NULL);
		std::cout << "col1: " << col1 << " col2: " << col2 << std::endl;
	}

	::SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	return true;
}

bool DBController::ReadDataFromDatabase(const wchar_t* query) {
	return FetchData(query);
}

bool DBController::WriteDataToDatabase(const wchar_t* query) {
	return ExecuteQuery(query);
}

// Define the function to print ODBC error information
void DBController::PrintSQLError(SQLSMALLINT handleType, SQLHANDLE handle)
{
	SQLSMALLINT i = 0;
	SQLINTEGER  nativeError;
	SQLWCHAR    sqlState[7];
	SQLWCHAR    messageText[256];
	SQLSMALLINT textLength;
	SQLRETURN   ret;

	do {
		i++;
		ret = SQLGetDiagRec(handleType, handle, i, sqlState, &nativeError, messageText, sizeof(messageText), &textLength);
		if (SQL_SUCCEEDED(ret)) {
			std::wcout << L"ODBC Error: SQL State: " << sqlState << L", Native Error: " << nativeError << L", Message: " << messageText << std::endl;
		}
	} while (ret == SQL_SUCCESS);
}

void DBController::CheckSQLReturn(SQLRETURN ret, SQLSMALLINT handleType, SQLHANDLE handle) 
{
	if (ret == SQL_SUCCESS) {
		std::cout << "Operation completed successfully." << std::endl;
	}
	else if (ret == SQL_SUCCESS_WITH_INFO) {
		std::cout << "Operation completed successfully with additional information." << std::endl;
		PrintSQLError(handleType, handle);
	}
	else if (ret == SQL_ERROR) {
		std::cerr << "Error in operation." << std::endl;
		PrintSQLError(handleType, handle);
	}
	else if (ret == SQL_INVALID_HANDLE) {
		std::cerr << "Invalid handle." << std::endl;
	}
	else if (ret == SQL_NO_DATA) {
		std::cerr << "No data found." << std::endl;
	}
	else {
		std::cerr << "Unknown return code: " << ret << std::endl;
	}
}