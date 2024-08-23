#pragma once
#include <cstdint> 

class ObjectTag : public DwordOverloader<ObjectTag> {
	DWORD_OVERLOADER(ObjectTag)

    static const DWORD Untagged         = 0x0000;
    static const DWORD Player           = 0x0001;
    static const DWORD Terrain          = 0x0002;
    static const DWORD Building         = 0x0004;
    static const DWORD Dynamic          = 0x0008;
    static const DWORD DissolveBuilding = 0x0010;
    static const DWORD Bullet           = 0x0020;
    static const DWORD Enemy            = 0x0040;
    static const DWORD Prop             = 0x0080;
    static const DWORD AfterStaticImage = 0x0100;
    static const DWORD AfterSkinImage   = 0x0200;
    static const DWORD Crate            = 0x0400;
    static const DWORD Item             = 0x0800;
    static const DWORD Bound            = 0x1000;
    static const DWORD Phero            = 0x2000;
    static const DWORD Skill            = 0x4000;

public:
    // Enable specific tag(s)
    void EnableTag(DWORD tag) { *this |= tag; }
    // Disable specific tag(s)
    void DisableTag(DWORD tag) { *this &= ~tag; }
    // Set the tag directly (overwrite current tag)
	void SetTag(DWORD tag) { *this = tag; }
    // Get the current tag(s)
	DWORD GetTag() const { return *this; }
    // Check if a specific tag is enabled
	bool IsTagEnabled(DWORD tag) const { return (*this & tag) != 0; }
};