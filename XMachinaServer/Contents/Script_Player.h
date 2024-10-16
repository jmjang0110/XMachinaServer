#pragma once

#include "Script_PlayerStat.h"
#include "ViewList.h"

class Component;
class ViewList;
class Script_Skill;

enum class PlayerState : UINT8 {
	None, 
	Lobby,
	InGame, 
	Exit,

	_count,
};

class GameSession;
class Script_Player : public Script_PlayerStat
{ 
private:
	SPtr<GameSession> mSessionOwner = nullptr;

private:
	std::array<SPtr<GameObject>, FBProtocol::PLAYER_SKILL_TYPE_END>		mSkills              = {};	

	SPtr<GameObject>													mDefaultWeapon       = {}; // H_Lock
	concurrency::concurrent_unordered_map<uint32_t, SPtr<GameObject>>	mWeapons;					Lock::SRWLock mWeapons_Lock;
	SPtr<GameObject>													mCurrWeapon          = {};	Lock::SRWLock mCurrWeapon_Lock;
	SPtr<GameObject>													mAirStrike			 = {};

	ViewList								                            mViewListSnapShot    = {};	Lock::SRWLock mViewList_Lock;
	ViewList															mViewList	         = {};
	PlayerState															mPlayerState         = PlayerState::None;

public:  
	Script_Player();
	Script_Player(SPtr<GameObject> owner);
	virtual ~Script_Player();

	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);

public: 
	virtual void Update();
	virtual void Start();

public:
	void	UpdateViewList(std::vector<SPtr<GameObject>> players, std::vector<SPtr<GameObject>> montser);
	void    CollideCheckWithMonsters();
	void    CollideCheckWithPheros(SPtr<GameObject> enemy);

	int		OnShoot(Vec3& bullet_center, Vec3& bullet_dir);
	int		OnHitEnemy(int32_t monster_id, Vec3& bullet_center, Vec3& bullet_dir);
	int		OnHitExpEnemy(int32_t monster_id);
	bool	OnSkill(FBProtocol::PLAYER_SKILL_TYPE type, SPtr<GameObject> mindControlledMonster = nullptr);

public:
	SPtr<Script_Skill>	GetSkillEntity(FBProtocol::PLAYER_SKILL_TYPE type);
	SPtr<GameObject>	GetSKill(FBProtocol::PLAYER_SKILL_TYPE type)			{ return mSkills[type]; }
	PlayerState			GetCurrState()											{ return mPlayerState; }
	SPtr<GameObject>	GetCurrWeapon()												{ mCurrWeapon_Lock.LockRead(); SPtr<GameObject> weapon = mCurrWeapon; mCurrWeapon_Lock.UnlockRead(); return mCurrWeapon; }
	SPtr<GameObject>	GetWeapon(uint32_t id);
	SPtr<GameSession>	GetSessionOwner()										{ return mSessionOwner; }
	ViewList			S_GetViewList()											{ mViewList_Lock.LockRead(); ViewList currViewList = mViewListSnapShot; mViewList_Lock.UnlockRead(); return currViewList; }

	void SetState(PlayerState state)				{ mPlayerState  = state; }
	void SetSessionOwner(SPtr<GameSession> session) { mSessionOwner = session; }
	void SetWeapon(SPtr<GameObject> weapon);
	void SetAirStrike();


public:
	void AddWeapon(SPtr<GameObject> weapon);
	void DropWeapon(uint32_t weapon_id);


};

