#include "pch.h"
#include <stdexcept>
#include "ComException.h"

void TestResult(const char* expression, HRESULT hr)
{
	if (FAILED(hr)) throw ComException(expression, hr);
}

std::string ComException::Message() const
{
	char buf[800];
	memset(buf, 0, sizeof(buf));
	sprintf_s(buf, "ComException hr:%x, Where:%s", hr, where.c_str());
	std::string str = buf;
	return str;
}

void ThrowRuntimeError(const char* error)
{
	OutputDebugStringA(error);
	throw std::runtime_error(error);
}

