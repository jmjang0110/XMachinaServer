#pragma once

class DB_Object
{
public:
	SQLHSTMT	mSQL_hStmt    = {};
	SQLRETURN	mSQL_Ret      = {};
	SQLSMALLINT mSQL_ColCount = 0;

public:
	DB_Object();
	~DB_Object();

public:
	virtual void FetchDataFromDataBase(const wchar_t* query); // DB 로 부터 데이터 읽는 전용 함수 

};

