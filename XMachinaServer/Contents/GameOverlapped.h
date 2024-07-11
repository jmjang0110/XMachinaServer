#pragma once
#include "ServerLib/OverlappedObject.h"
#include "ServerLib/MemoryManager.h"

namespace OverlappedEx {
	enum class Type  : UINT8 {

		GAMEWORLD_UPDATE_IO_ENUM_START = OverlappedIO::Type::NETWORK_IO_ENUM_END,

		/* Game World Update IO */
		GameObject_Update,


	};
}

/* GameObject Update By Worker Thread */
class Overlapped_GameObject_Update : public OverlappedObject
{

public:
	Overlapped_GameObject_Update()
		: OverlappedObject(static_cast<OverlappedIO::Type>(OverlappedEx::Type::GameObject_Update))
	{
	}

	~Overlapped_GameObject_Update()
	{
		MEMORY->Delete(this);

	}

};
