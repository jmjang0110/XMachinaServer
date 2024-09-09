#include "pch.h"
#include "DB_Object.h"
#include "DBController.h"

DB_Object::DB_Object()
{
}

DB_Object::~DB_Object()
{
}

void DB_Object::FetchDataFromDataBase(const wchar_t* query)
{
	mSQL_hStmt    = {};
	mSQL_Ret      = {};

	// ���� �ڵ� �Ҵ�
	mSQL_Ret = ::SQLAllocHandle(SQL_HANDLE_STMT, DB_CONTROLLER->SQLH_Dbc(), &mSQL_hStmt);
	if (mSQL_Ret != SQL_SUCCESS && mSQL_Ret != SQL_SUCCESS_WITH_INFO) {
		LOG_MGR->Cout("ȯ�� �ڵ� �Ҵ� ����\n");
	}

	// ���� ����
	mSQL_Ret = SQLExecDirect(mSQL_hStmt, (SQLWCHAR*)query, SQL_NTS);
	if (mSQL_Ret != SQL_SUCCESS && mSQL_Ret != SQL_SUCCESS_WITH_INFO) {
		LOG_MGR->Cout("���� ���� ����\n");
		::SQLFreeHandle(SQL_HANDLE_STMT, mSQL_hStmt);
	}

}

