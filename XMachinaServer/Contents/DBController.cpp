#include "pch.h"
#include "DBController.h"
#include "ServerLib/MemoryManager.h"

#include "GameObject.h"
#include "Transform.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "Animation.h"

DEFINE_SINGLETON(DBController);

DBController::DBController()
{

}

DBController::~DBController()
{
	SQLFreeHandle(SQL_HANDLE_ENV, mhEnv);
}

void DBController::Init()
{

}

void DBController::Launch()
{
	while (mLaunchThread) {
		DataBaseEvent ev;
		/* 처리할 Event 를 꺼낸다. */
		if (mDBEventPQ.try_pop(ev)) {

			/* DataBase Event 실행 */
			Process_DataBaseEvent(ev);
		}
	}
}

void DBController::Process_DataBaseEvent(DataBaseEvent ev)
{
	DataBaseEventType type = ev.DBEventType;
	switch (type)
	{
	case DataBaseEventType::Query: {
		ev.DBObject->FetchDataFromDataBase(ev.Query.c_str());
	}
		break;
	default:
		assert(0);
		break;
	}
}

void DBController::PushDataBaseEvent(DataBaseEvent ev)
{
	mDBEventPQ.push(ev);
}

void DBController::PushDataBaseEvent(QueryPriority queryPriority, DataBaseEventType evType, SPtr<DB_Object> dbObj, const wchar_t* query)
{
	DataBaseEvent ev{};
	ev.QPriority   = queryPriority;
	ev.DBEventType = evType;
	ev.DBObject    = dbObj;
	ev.Query       = query;

	PushDataBaseEvent(ev);
}

void DBController::ExecuteAllDataBaseEvents()
{
	while (!mDBEventPQ.empty()) {
		DataBaseEvent ev;
		/* 처리할 Event 를 꺼낸다. */
		if (mDBEventPQ.try_pop(ev)) {

			/* DataBase Event 실행 */
			Process_DataBaseEvent(ev);
		}
	}
}


bool DBController::ConnectToDatabase(const wchar_t* dsn, const wchar_t* user, const wchar_t* password) {
	SQLRETURN ret;

	// 환경 핸들 할당
	ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &mhEnv);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		LOG_MGR->Cout("환경 핸들 할당 실패\n");
		return false;
	}

	// ODBC 버전 설정
	ret = ::SQLSetEnvAttr(mhEnv, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), 0);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		LOG_MGR->Cout("ODBC 버전 설정 실패\n");
		::SQLFreeHandle(SQL_HANDLE_ENV, mhEnv);
		return false;
	}

	// 연결 핸들 할당
	ret = ::SQLAllocHandle(SQL_HANDLE_DBC, mhEnv, &mhDbc);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		LOG_MGR->Cout("연결 핸들 할당 실패\n");
		::SQLFreeHandle(SQL_HANDLE_ENV, mhEnv);
		return false;
	}

	// 데이터베이스 연결
	ret = ::SQLConnect(mhDbc, (SQLWCHAR*)dsn, SQL_NTS, (SQLWCHAR*)user, SQL_NTS, (SQLWCHAR*)password, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		CheckSQLReturn(ret, SQL_HANDLE_DBC, mhDbc);
		LOG_MGR->Cout("데이터베이스 연결 실패\n");
		::SQLFreeHandle(SQL_HANDLE_DBC, mhDbc);
		::SQLFreeHandle(SQL_HANDLE_ENV, mhEnv);
		return false;
	}

	std::cout << "데이터베이스 연결 성공" << std::endl;
	return true;
}

void DBController::DisconnectFromDatabase() {
	if (mhDbc != SQL_NULL_HDBC) {
		::SQLDisconnect(mhDbc);
		::SQLFreeHandle(SQL_HANDLE_DBC, mhDbc);
	}
	if (mhEnv != SQL_NULL_HENV) {
		::SQLFreeHandle(SQL_HANDLE_ENV, mhEnv);
	}
}

bool DBController::ExecuteQuery(const wchar_t* query) {
	SQLHSTMT hStmt;
	SQLRETURN ret;

	// 문장 핸들 할당
	ret = ::SQLAllocHandle(SQL_HANDLE_STMT, mhDbc, &hStmt);
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
	SQLHSTMT    hStmt = {};
	SQLRETURN   ret = {};
	SQLSMALLINT colCount = 0;

	// 문장 핸들 할당
	ret = ::SQLAllocHandle(SQL_HANDLE_STMT, mhDbc, &hStmt);
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

	// 컬럼 개수 가져오기
	ret = SQLNumResultCols(hStmt, &colCount);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		LOG_MGR->Cout("컬럼 개수 가져오기 실패\n");
		::SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		return false;
	}

	// 데이터 페칭
	while ((ret = SQLFetch(hStmt)) == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
		for (SQLLEN i = 1; i <= colCount; ++i) {
			SQLWCHAR	colName[256] = {};
			SQLSMALLINT colNameLen, colType, colScale, colNullable;
			SQLULEN		colSize;

			// 컬럼 메타 정보 가져오기
			ret = SQLDescribeCol(hStmt, i, colName, sizeof(colName), &colNameLen, &colType, &colSize, &colScale, &colNullable);
			if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
				LOG_MGR->Cout("컬럼 설명 실패\n");
				::SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
				return false;
			}

			SQLLEN colDataLen = 0;

			// 데이터 타입에 따라 적절한 C 타입 지정
			switch (colType) {
			case SQL_CHAR:
			case SQL_VARCHAR:
			case SQL_LONGVARCHAR: {
				SQLCHAR colData[256] = {};
				ret = SQLGetData(hStmt, i, SQL_C_CHAR, colData, sizeof(colData), &colDataLen);
				std::cout << "col" << i << ": " << colData << std::endl;
				break;
			}
			case SQL_INTEGER: {
				SQLINTEGER intData = 0;
				ret = SQLGetData(hStmt, i, SQL_C_SLONG, &intData, sizeof(intData), &colDataLen);
				std::cout << "col" << i << ": " << intData << std::endl;
				break;
			}
			case SQL_FLOAT: {
				SQLFLOAT floatData = 0.0f;
				ret = SQLGetData(hStmt, i, SQL_C_DOUBLE /*SQL_C_FLOAT*/, &floatData, sizeof(floatData), &colDataLen);
				std::cout << "col" << i << ": " << floatData << std::endl;
				break;
			}
			case SQL_DOUBLE:
			case SQL_REAL: {
				SQLDOUBLE doubleData = 0;
				ret = SQLGetData(hStmt, i, SQL_C_DOUBLE, &doubleData, sizeof(doubleData), &colDataLen);
				std::cout << "col" << i << ": " << doubleData << std::endl;
				break;
			}
						 // 추가적인 데이터 타입 처리가 필요하다면 여기에 추가
			default: {
				LOG_MGR->Cout("지원하지 않는 데이터 타입\n");
				::SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
				return false;
			}
			}

			// SQLGetData 성공 여부 확인
			if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
				LOG_MGR->Cout("데이터 가져오기 실패\n");
				std::cout << "SQLGetData for INTEGER failed with ret: " << ret << std::endl;

				SQLWCHAR sqlState[6];
				SQLWCHAR messageText[SQL_MAX_MESSAGE_LENGTH];
				SQLSMALLINT textLength;
				SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, sqlState, nullptr, messageText, sizeof(messageText), &textLength);
				std::wcout << "SQL Error State: " << sqlState << ", Message: " << messageText << std::endl;

				::SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
				return false;
			}
		}
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
		std::cout << "Error in operation." << std::endl;
		PrintSQLError(handleType, handle);
	}
	else if (ret == SQL_INVALID_HANDLE) {
		std::cout << "Invalid handle." << std::endl;
	}
	else if (ret == SQL_NO_DATA) {
		std::cout << "No data found." << std::endl;
	}
	else {
		std::cout << "Unknown return code: " << ret << std::endl;
	}
}