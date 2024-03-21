#include "pch.h"
#include "Session.h"

Session::Session()
{
}

Session::~Session()
{
}

void Session::RegisterIO(OverlappedIO::Type IoType)
{
	switch (IoType)
	{
	case OverlappedIO::Type::Accept:
	{

	}
		break;
	case OverlappedIO::Type::Connect:
	{

	}
		break;
	case OverlappedIO::Type::DisConnect:
	{

	}
		break;
	case OverlappedIO::Type::Send:
	{

	}
		break;
	case OverlappedIO::Type::Recv:
	{

	}
		break;
	}

}

void Session::ProcessIO(OverlappedIO::Type IoType)
{
	switch (IoType)
	{
	case OverlappedIO::Type::Accept:
	{

	}
		break;
	case OverlappedIO::Type::Connect:
	{

	}
		break;
	case OverlappedIO::Type::DisConnect:
	{

	}
		break;
	case OverlappedIO::Type::Send:
	{

	}
		break;
	case OverlappedIO::Type::Recv:
	{

	}
		break;
	}
}
