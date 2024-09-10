#pragma once
#include "DB_Object.h"

class DB_Phero : public DB_Object
{
public:
	int		PK_Level;
	float	LifeTime_second;
	float	Amount;

public:
	DB_Phero();
	~DB_Phero();

public:
	virtual void FetchDataFromDataBase(const wchar_t* query); // DB 로 부터 데이터 읽는 전용 함수 
	void LoadFromDataBase(int PK_Level);

};

