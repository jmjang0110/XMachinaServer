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
	AdvancedCombatDroid_5

};

/* Monster Prototype Information */
struct MonsterPrototypeDataBase 
{
	std::string name;

	float HP;
	float MP;
	float attack;

};

class DB_Monster
{
private:

public:
	DB_Monster();
	~DB_Monster();



};

