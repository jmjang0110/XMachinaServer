#pragma once
#include "DB_Object.h"

class DB_PheroDropInfo : public DB_Object
{
public:
	int		PK_Level          = {};
	int		MinPheroDropCount = {};
	int		MaxPheroDropCount = {};
	float	L1PheroDropRate   = {};
	float	L2PheroDropRate   = {};
	float	L3PheroDropRate   = {};

public:
	DB_PheroDropInfo();
	~DB_PheroDropInfo();

public:
	virtual void FetchDataFromDataBase(const wchar_t* query); // DB �� ���� ������ �д� ���� �Լ� 
	void LoadFromDataBase(int PK_Level);

};
