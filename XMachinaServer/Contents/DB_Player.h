#pragma once
#include "DB_Object.h"

class DB_Player : public DB_Object
{
public:

public:
	DB_Player();
	~DB_Player();

public:
	virtual void FetchDataFromDataBase(const wchar_t* query); // DB 로 부터 데이터 읽는 전용 함수 
	void LoadFromDataBase(int PK_ID);

};



