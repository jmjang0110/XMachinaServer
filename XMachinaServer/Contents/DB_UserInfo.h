#pragma once
#include "DB_Object.h"

class GameSession;
class DB_UserInfo : public DB_Object
{
public:
	SPtr<GameSession> mOwnerSession = nullptr;

public:
	bool		IsLogInSuccess = false;
	std::string Name{};
	std::string ID{};
	std::string Password{};


public:
	DB_UserInfo();
	~DB_UserInfo();

public:
	virtual void FetchDataFromDataBase(const wchar_t* query); // DB �� ���� ������ �д� ���� �Լ� 
	void LoadFromDataBase(std::string PK_ID, std::string inputPassword);

public:
	bool IsLoginSuccessful(const std::string& inputID, const std::string& inputPassword);
	void SetOnwerGameSession(SPtr<GameSession> session) { mOwnerSession = session; }
	
	
		
};



