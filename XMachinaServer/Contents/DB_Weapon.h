#pragma once
#include "DB_Object.h"


namespace WeaponInfo {
	enum class BulletPSType : UINT8 {
		Explosion = 0,
		Contrail,
		Building,

		_count
	};
	enum { BulletPSTypeCount = static_cast<UINT8>(BulletPSType::_count) };

	enum class WeaponType {
		HandedGun,
		AssaultRifle,
		ShotGun,
		MissileLauncher,
		Sniper,
		_count
	};

	enum class WeaponName {
		None,

		// HandedGun
		H_Lock,

		// Shotgun
		DBMS,

		// AssaultRifle
		SkyLine,

		// MissileLaunchere
		Burnout,

		// Sniper
		PipeLine,

		_count,
	};
	static const std::unordered_map<WeaponInfo::WeaponName, WeaponType> kWeaponTypes{
		{WeaponName::H_Lock,	WeaponType::HandedGun },
		{WeaponName::DBMS,		WeaponType::ShotGun },
		{WeaponName::SkyLine,	WeaponType::AssaultRifle },
		{WeaponName::Burnout,	WeaponType::MissileLauncher },
		{WeaponName::PipeLine,	WeaponType::Sniper },
	};

	static const std::unordered_map<WeaponType, std::string> kDefaultTransforms{
		{WeaponType::HandedGun, "RefPos2HandedGun_Action" },
		{WeaponType::AssaultRifle, "RefPosAssaultRifle_Action" },
		{WeaponType::ShotGun, "RefPosShotgun_Action" },
		{WeaponType::MissileLauncher, "RefPosMissileLauncher_Action" },
		{WeaponType::Sniper, "RefPosSniper_Action" },
	};
}

class DB_Weapon : public DB_Object
{
public:
	float	AttackRate           = {};
	float   MaxFireDelay         = {};
	float	MaxReloadTime        = {};
	float	MaxDistance          = {};
	int		MaxMag               = {};
	int		BulletCountPerMag    = {};
	int		BulletCountPerShot   = {};

public:
	DB_Weapon();
	~DB_Weapon();

public:
	virtual void FetchDataFromDataBase(const wchar_t* query); // DB 로 부터 데이터 읽는 전용 함수 
	void LoadFromDataBase(const wchar_t* weapon_name );


};

