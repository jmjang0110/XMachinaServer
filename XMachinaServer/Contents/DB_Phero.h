#pragma once
#include "DB_Object.h"

class DB_Phero : public DB_Object
{
public:
	int		Level;
	float	LifeTime_second;
	float	Amount;

public:
	DB_Phero();
	~DB_Phero();

public:
	virtual void FetchDataFromDataBase(const wchar_t* query); // DB �� ���� ������ �д� ���� �Լ� 
	void FetchPheroData(int level);

};

