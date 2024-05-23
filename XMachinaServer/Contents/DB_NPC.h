#pragma once

/// +-------------------------------
///	       NPC Data Base 
/// ________________________________
/// Manage NPC Data In Game World 
/// -------------------------------+

struct NPCDataBase
{
	std::string NPCID;
	std::string name;
	
	Vec3 Position;
};
class DB_NPC
{
private:


public:
	DB_NPC();
	~DB_NPC();


};

