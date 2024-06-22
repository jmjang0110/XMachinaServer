#pragma once

#include "GameEntity.h"

/// +-------------------------------
///		        Sector
/// ________________________________
///
/// 지형을 여러 Sector 로 나눈다.
/// 
/// 24.06.22 : 어떤 Player 들이 들어와 있는지 관리하자.!! 
/// -------------------------------+

class Sector : public GameEntity
{
private:


public:

	Sector();
	Sector(int id);
	virtual ~Sector() override;

	
};

