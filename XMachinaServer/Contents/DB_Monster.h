#pragma once

/// +-------------------------------
///	     Monster Data Base 
/// ________________________________
/// Manage Monster Data In Game World 
/// -------------------------------+

enum class MonsterType : UINT8
{
	Ursacetus,
	Onyscidus,
	AdvancedCombatDroid_5,
	Anglerox,
	Arack,
	Ceratoferox,
	Gobbler,
	LightBipedMech,
	MiningMech,
	Rapax,
	Aranobot,

	End,

};

/* Monster Prototype Information */
struct MonsterDataBase 
{
	std::string name;

	float HP;
	float MP;
	float attack;

};

/* Prototype Information */
class DB_Monster
{
private:
	MonsterType		mType;
	MonsterDataBase mProtoDB;

public:
	DB_Monster();
	~DB_Monster();

};

