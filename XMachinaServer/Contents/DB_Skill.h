#pragma once

#include "DB_Object.h"

class GameObject;
class DB_Skill : public DB_Object
{
public:
	std::string	Name{};
	float			CoolTime{};
	float			ActiveDurationTime{};

public:
	DB_Skill();
	~DB_Skill();

public:
	virtual void FetchDataFromDataBase(const wchar_t* query); // DB �� ���� ������ �д� ���� �Լ� 
	void LoadFromDataBase(std::string PK_Name);

};

