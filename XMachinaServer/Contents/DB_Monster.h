#pragma once

/// +-------------------------------
///	     Monster Data Base 
/// ________________________________
/// Manage Monster Data In Game World 
/// -------------------------------+
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
	MonsterDataBase mProtoDB;

public:
	DB_Monster();
	~DB_Monster();

};

