#include "pch.h"
#include "OverlappedObject.h"

/* Overlapped Object */
OverlappedObject::OverlappedObject(OverlappedIO::Type ioType) : mIoType(ioType)
{
	::ZeroMemory(this, sizeof(OVERLAPPED));
}

OverlappedObject::~OverlappedObject()
{
}
	