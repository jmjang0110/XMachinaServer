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

	// 문장 핸들 할당
	mSQL_Ret = ::SQLAllocHandle(SQL_HANDLE_STMT, DB_CONTROLLER->SQLH_Dbc(), &mSQL_hStmt);
	if (mSQL_Ret != SQL_SUCCESS && mSQL_Ret != SQL_SUCCESS_WITH_INFO) {
		LOG_MGR->Cout("환경 핸들 할당 실패\n");
	}

	// 쿼리 실행
	mSQL_Ret = SQLExecDirect(mSQL_hStmt, (SQLWCHAR*)query, SQL_NTS);
	if (mSQL_Ret != SQL_SUCCESS && mSQL_Ret != SQL_SUCCESS_WITH_INFO) {
		LOG_MGR->Cout("쿼리 실행 실패\n");
		::SQLFreeHandle(SQL_HANDLE_STMT, mSQL_hStmt);
	}

}

