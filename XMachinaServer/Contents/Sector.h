#pragma once

#include "GameEntity.h"

/// +-------------------------------
///		        Sector
/// ________________________________
///
/// ������ ���� Sector �� ������.
/// 
/// 24.06.22 : � Player ���� ���� �ִ��� ��������.!! 
/// -------------------------------+

class Sector : public GameEntity
{
private:


public:

	Sector();
	Sector(int id);
	virtual ~Sector() override;

	
};

