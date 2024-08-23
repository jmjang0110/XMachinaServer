#pragma once
#include "DwordOverloader.h"

class EntityTag : public DwordOverloader<EntityTag> {
	DWORD_OVERLOADER(EntityTag)

	static const DWORD Untagged			= 0x0000;
	static const DWORD Player			= 0x0001;
	static const DWORD Terrain			= 0x0002;
	static const DWORD Building			= 0x0004;
	static const DWORD Dynamic			= 0x0008;
	static const DWORD DissolveBuilding = 0x0010;
	static const DWORD Bullet			= 0x0020;
	static const DWORD Enemy			= 0x0040;
	static const DWORD Prop				= 0x0080;
	static const DWORD AfterStaticImage	= 0x0100;
	static const DWORD AfterSkinImage	= 0x0200;
	static const DWORD Crate			= 0x0400;
	static const DWORD Item				= 0x0800;
	static const DWORD Bound			= 0x1000;
};

