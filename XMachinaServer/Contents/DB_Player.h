#pragma once
#include "DB_Object.h"

class DB_Player : public DB_Object
{
public:

public:
	DB_Player();
	~DB_Player();

public:
	virtual void FetchDataFromDataBase(const wchar_t* query); // DB �� ���� ������ �д� ���� �Լ� 
	void LoadFromDataBase(int PK_ID);

};



