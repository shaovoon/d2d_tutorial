#pragma once
#include <Windows.h>
#include <string>

void TestResult(const char* expression, HRESULT hr);

struct ComException
{
	HRESULT const hr;
	std::string where;
	ComException(const char* expression, HRESULT const value) : where(expression), hr(value) {}
	std::string Message() const;
};


void ThrowRuntimeError(const char* error);