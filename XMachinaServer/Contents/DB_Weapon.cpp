#include "pch.h"
#include "DB_Weapon.h"
#include "DBController.h"

DB_Weapon::DB_Weapon() 
	: DB_Object()

{
}

DB_Weapon::~DB_Weapon()
{
}

void DB_Weapon::FetchDataFromDataBase(const wchar_t* query)
{
    // �⺻ Ŭ������ �޼��带 ȣ���Ͽ� Statement �ڵ��� �غ�
    DB_Object::FetchDataFromDataBase(query);

    // ������ ��������
    wchar_t name[50];  // ���� �̸��� ������ �迭

    while ((mSQL_Ret = SQLFetch(mSQL_hStmt)) == SQL_SUCCESS || mSQL_Ret == SQL_SUCCESS_WITH_INFO) {
        SQLGetData(mSQL_hStmt, 1, SQL_C_WCHAR, name, sizeof(name), nullptr);      // Name (nvarchar(50))
        SQLGetData(mSQL_hStmt, 2, SQL_C_FLOAT, &AttackRate, 0, nullptr);          // Attack
        SQLGetData(mSQL_hStmt, 3, SQL_C_FLOAT, &MaxFireDelay, 0, nullptr);        // MaxFireDelay
        SQLGetData(mSQL_hStmt, 4, SQL_C_FLOAT, &MaxReloadTime, 0, nullptr);       // MaxReloadTime
        SQLGetData(mSQL_hStmt, 5, SQL_C_SLONG, &MaxMag, 0, nullptr);              // MaxMag
        SQLGetData(mSQL_hStmt, 6, SQL_C_SLONG, &BulletCountPerShot, 0, nullptr);  // BulletCountPerShot
        SQLGetData(mSQL_hStmt, 7, SQL_C_SLONG, &BulletCountPerMag, 0, nullptr);   // BulletCountPerMag
    }

    // �ڵ� ����
    ::SQLFreeHandle(SQL_HANDLE_STMT, mSQL_hStmt);

    // ������ �����͸� ��� (�׽�Ʈ��)
    std::wcout << L"Name: " << name << L", Attack: " << AttackRate << L", MaxFireDelay: " << MaxFireDelay
        << L", MaxReloadTime: " << MaxReloadTime << L", MaxMag: " << MaxMag
        << L", BulletCountPerMag: " << BulletCountPerMag << L", BulletCountPerShot: " << BulletCountPerShot << std::endl;
}


void DB_Weapon::LoadFromDataBase(const wchar_t* weaponName)
{
    // ���� �ۼ� (���� �̸��� �������� ������ ��ȸ)
    wchar_t query[256];
    swprintf(query, 256, L"SELECT Name, AttackRate, MaxFireDelay, MaxReloadTime, MaxMag, BulletCountPerShot, BulletCountPerMag FROM GunWeapon WHERE Name = '%s'", weaponName);

    DB_CONTROLLER->PushDataBaseEvent(QueryPriority::None, DataBaseEventType::Query, shared_from_this(), query);
}


