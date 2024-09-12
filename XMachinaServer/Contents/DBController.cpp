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
		/* ó���� Event �� ������. */
		if (mDBEventPQ.try_pop(ev)) {

			/* DataBase Event ���� */
			Process_DataBaseEvent(ev);
		}
	}
}

void DBController::Process_DataBaseEvent(DataBaseEvent ev)
{
	DataBaseEventType type = ev.DBEventType;
	switch (type)
	{
	case DataBaseEventType::None: {

	}
		break;
	case DataBaseEventType::Query: {

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

bool DBController::ConnectToDatabase(const wchar_t* dsn, const wchar_t* user, const wchar_t* password) {
	SQLRETURN ret;

	// ȯ�� �ڵ� �Ҵ�
	ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &mhEnv);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		LOG_MGR->Cout("ȯ�� �ڵ� �Ҵ� ����\n");
		return false;
	}

	// ODBC ���� ����
	ret = ::SQLSetEnvAttr(mhEnv, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), 0);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		LOG_MGR->Cout("ODBC ���� ���� ����\n");
		::SQLFreeHandle(SQL_HANDLE_ENV, mhEnv);
		return false;
	}

	// ���� �ڵ� �Ҵ�
	ret = ::SQLAllocHandle(SQL_HANDLE_DBC, mhEnv, &mhDbc);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		LOG_MGR->Cout("���� �ڵ� �Ҵ� ����\n");
		::SQLFreeHandle(SQL_HANDLE_ENV, mhEnv);
		return false;
	}

	// �����ͺ��̽� ����
	ret = ::SQLConnect(mhDbc, (SQLWCHAR*)dsn, SQL_NTS, (SQLWCHAR*)user, SQL_NTS, (SQLWCHAR*)password, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		CheckSQLReturn(ret, SQL_HANDLE_DBC, mhDbc);
		LOG_MGR->Cout("�����ͺ��̽� ���� ����\n");
		::SQLFreeHandle(SQL_HANDLE_DBC, mhDbc);
		::SQLFreeHandle(SQL_HANDLE_ENV, mhEnv);
		return false;
	}

	std::cout << "�����ͺ��̽� ���� ����" << std::endl;
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

	// ���� �ڵ� �Ҵ�
	ret = ::SQLAllocHandle(SQL_HANDLE_STMT, mhDbc, &hStmt);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		LOG_MGR->Cout("���� �ڵ� �Ҵ� ����\n");
		return false;
	}

	// ���� ����
	ret = ::SQLExecDirect(hStmt, (SQLWCHAR*)query, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		LOG_MGR->Cout("���� ���� ����\n");
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

	// ���� �ڵ� �Ҵ�
	ret = ::SQLAllocHandle(SQL_HANDLE_STMT, mhDbc, &hStmt);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		LOG_MGR->Cout("ȯ�� �ڵ� �Ҵ� ����\n");
		return false;
	}

	// ���� ����
	ret = SQLExecDirect(hStmt, (SQLWCHAR*)query, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		LOG_MGR->Cout("���� ���� ����\n");
		::SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		return false;
	}

	// �÷� ���� ��������
	ret = SQLNumResultCols(hStmt, &colCount);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		LOG_MGR->Cout("�÷� ���� �������� ����\n");
		::SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		return false;
	}

	// ������ ��Ī
	while ((ret = SQLFetch(hStmt)) == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
		for (SQLLEN i = 1; i <= colCount; ++i) {
			SQLWCHAR	colName[256] = {};
			SQLSMALLINT colNameLen, colType, colScale, colNullable;
			SQLULEN		colSize;

			// �÷� ��Ÿ ���� ��������
			ret = SQLDescribeCol(hStmt, i, colName, sizeof(colName), &colNameLen, &colType, &colSize, &colScale, &colNullable);
			if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
				LOG_MGR->Cout("�÷� ���� ����\n");
				::SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
				return false;
			}

			SQLLEN colDataLen = 0;

			// ������ Ÿ�Կ� ���� ������ C Ÿ�� ����
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
						 // �߰����� ������ Ÿ�� ó���� �ʿ��ϴٸ� ���⿡ �߰�
			default: {
				LOG_MGR->Cout("�������� �ʴ� ������ Ÿ��\n");
				::SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
				return false;
			}
			}

			// SQLGetData ���� ���� Ȯ��
			if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
				LOG_MGR->Cout("������ �������� ����\n");
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