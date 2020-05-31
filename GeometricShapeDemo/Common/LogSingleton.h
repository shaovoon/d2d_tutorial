#pragma once
#include "Writer/xTextWriter.h"
#include "UtilityWin32.h"
#include "VariaNetDebugPrint.h"
#include "Utility.h"

enum class LogType
{
	Log,
	Debug,
	Warning,
	Error,
};

const std::wstring ConvLogType(LogType type_);

class LogSingleton
{
public:
	template<typename... Args>
	static void Log(LogType type_, const wchar_t* s, Args... args)
	{
		using namespace Library;
		Create();
		if (type_ == LogType::Debug)
		{
			DebugPrint(s, args...);
		}
		m_pWriter->Write(L"{0} {1}:", Utility::CurrentDateTimeW(), ConvLogType(type_));
		m_pWriter->WriteLine(s, args...);
	}

	static void Log(LogType type_, const wchar_t* s);
	
	static void Destroy();

private:
	LogSingleton() = delete;
	static void Create();
	static Elmax::xTextWriter* m_pWriter;
};

