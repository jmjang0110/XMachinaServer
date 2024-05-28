#include "pch.h"
#include "DBController.h"

DEFINE_SINGLETON(DBController);

DBController::DBController()
{
}

DBController::~DBController()
{
}

bool DBController::ConnectToDatabase(const char* dsn, const char* user, const char* password) {
	SQLRETURN ret;

	// 환경 핸들 할당
	ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		std::cerr << "환경 핸들 할당 실패" << std::endl;
		return false;
	}

	// ODBC 버전 설정
	ret = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		std::cerr << "ODBC 버전 설정 실패" << std::endl;
		SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
		return false;
	}

	// 연결 핸들 할당
	ret = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		std::cerr << "연결 핸들 할당 실패" << std::endl;
		SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
		return false;
	}

	// 데이터베이스 연결
	ret = SQLConnect(hDbc, (SQLWCHAR*)dsn, SQL_NTS, (SQLWCHAR*)user, SQL_NTS, (SQLWCHAR*)password, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		std::cerr << "데이터베이스 연결 실패" << std::endl;
		SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
		SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
		return false;
	}

	std::cout << "데이터베이스 연결 성공" << std::endl;
	return true;
}

void DBController::DisconnectFromDatabase() {
	if (hDbc != SQL_NULL_HDBC) {
		SQLDisconnect(hDbc);
		SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
	}
	if (hEnv != SQL_NULL_HENV) {
		SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
	}
}

bool DBController::ExecuteQuery(const char* query) {
	SQLHSTMT hStmt;
	SQLRETURN ret;

	// 문장 핸들 할당
	ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		std::cerr << "문장 핸들 할당 실패" << std::endl;
		return false;
	}

	// 쿼리 실행
	ret = SQLExecDirect(hStmt, (SQLWCHAR*)query, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		std::cerr << "쿼리 실행 실패" << std::endl;
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		return false;
	}

	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	return true;
}

bool DBController::FetchData(const char* query) {
	SQLHSTMT hStmt;
	SQLRETURN ret;
	SQLCHAR col1[256];
	SQLCHAR col2[256];

	// 문장 핸들 할당
	ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		std::cerr << "문장 핸들 할당 실패" << std::endl;
		return false;
	}

	// 쿼리 실행
	ret = SQLExecDirect(hStmt, (SQLWCHAR*)query, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		std::cerr << "쿼리 실행 실패" << std::endl;
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		return false;
	}

	// 데이터 페칭
	while (SQLFetch(hStmt) == SQL_SUCCESS) {
		SQLGetData(hStmt, 1, SQL_C_CHAR, col1, sizeof(col1), NULL);
		SQLGetData(hStmt, 2, SQL_C_CHAR, col2, sizeof(col2), NULL);
		std::cout << "col1: " << col1 << " col2: " << col2 << std::endl;
	}

	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	return true;
}

bool DBController::ReadDataFromDatabase(const char* query) {
	return FetchData(query);
}

bool DBController::WriteDataToDatabase(const char* query) {
	return ExecuteQuery(query);
}