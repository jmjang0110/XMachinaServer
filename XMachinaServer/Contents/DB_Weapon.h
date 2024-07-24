#pragma once
#include "DB_Item.h"



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

class DB_Weapon : public DB_Item
{


};

