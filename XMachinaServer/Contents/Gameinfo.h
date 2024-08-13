#pragma once


namespace GameObjectInfo
{
	constexpr int maxBulletsNum = 100;

	enum class Type : UINT16 {
		None,

		/// +------------------------
		///		 GAME PLAYER 
		/// ------------------------+
		GamePlayer,

		/// +------------------------
		///		 MONSTER TYPE 
		/// ------------------------+
		Monster_Ursacetus,
		Monster_Onyscidus,
		Monster_AdvancedCombat_5,
		Monster_Anglerox,
		Monster_Arack,
		Monster_Ceratoferox,
		Monster_Gobbler,
		Monster_LightBipedMech,
		Monster_MiningMech,
		Monster_Rapax,
		Monster_Aranobot,
		Montser_Deus_Phase_1,
		Monster_Deus_Phase_2,

		/// +------------------------
		///		 BUILDING 
		/// ------------------------+
		Building,

		/// +------------------------
		///		 BUILDING 
		/// ------------------------+
		Bullet,

		/// +------------------------
		///		 COLLIDER TYPE 
		/// ------------------------+
		BoxCollider,
		SphereCollider,
		ObjectCollider,
		Crate,
		END,
	};
}



class Gameinfo
{
};

