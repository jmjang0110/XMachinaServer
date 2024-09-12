#pragma once

class DB_Object : public std::enable_shared_from_this<DB_Object>
{
public:
	SQLHSTMT	mSQL_hStmt    = {};
	SQLRETURN	mSQL_Ret      = {};

public:
	DB_Object();
	~DB_Object();

public:
	virtual void FetchDataFromDataBase(const wchar_t* query); // DB �� ���� ������ �д� ���� �Լ� 
};

