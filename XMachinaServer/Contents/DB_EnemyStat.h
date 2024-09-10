#pragma once
#include "DB_Object.h"

class DB_EnemyStat : public DB_Object
{
public:
    char PK_Name[50];

    int         Level;
    int         PheroLevel;
    float       MoveSpeed;
    float       RotationSpeed;
    float       AttackRotationSpeed;

    float       DetectionRange;
    float       AttackRate;
    float       AttackRange;
    float       AttackCoolTime;

    char Attack1AnimName[50];
    char Attack2AnimName[50];
    char Attack3AnimName[50];
    char GetHitAnimName[50];
    char DeathAnimName[50];

public:
	DB_EnemyStat();
	~DB_EnemyStat();

public:
	virtual void FetchDataFromDataBase(const wchar_t* query); // DB 로 부터 데이터 읽는 전용 함수 
	void LoadFromDataBase(std::string PK_Name);

};

