#pragma once
#include "ServerLib/OverlappedObject.h"

namespace OverlappedEx {
	enum class Type  : UINT8 {

		GAMEWORLD_UPDATE_IO_ENUM_START = OverlappedIO::Type::NETWORK_IO_ENUM_END,

		/* Game World Update IO */
		Monster_Update,


	};
}

/* Accept Overlapped Obeject */
class Overlapped_Monster_Update : public OverlappedObject
{

public:
	Overlapped_Monster_Update()
		: OverlappedObject(static_cast<OverlappedIO::Type>(OverlappedEx::Type::Monster_Update))
	{
	}

	~Overlapped_Monster_Update()
	{
	}

};
