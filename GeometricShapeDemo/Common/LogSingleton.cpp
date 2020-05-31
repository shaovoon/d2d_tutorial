#include "pch.h"
#include "LogSingleton.h"

Elmax::xTextWriter* LogSingleton::m_pWriter=nullptr;

const std::wstring ConvLogType(LogType type_)
{
	switch (type_)
	{
	case LogType::Log:
		return L"[Log]";
	case LogType::Debug:
		return L"[Debug]";
	case LogType::Warning:
		return L"[Warning]";
	case LogType::Error:
		return L"[Error]";
	default:
		return L"[]";
	}
	return L"[]";
}

void LogSingleton::Log(LogType type_, const wchar_t* s)
{
	using namespace Library;
	Create();
	if (type_ == LogType::Debug)
	{
		DebugPrint(s);
	}
	m_pWriter->Write(L"{0} {1}:", Utility::CurrentDateTimeW(), ConvLogType(type_));
	m_pWriter->WriteLine(s);
}

void LogSingleton::Create()
{
	if (!m_pWriter)
	{
		m_pWriter = new Elmax::xTextWriter();
		using namespace Library;
		std::wstring file = UtilityWin32::CombinePath(UtilityWin32::LogFolder(), L"Log.txt");
		m_pWriter->Open(file, FT_UNICODE, APPEND);
	}
}

void LogSingleton::Destroy()
{
	if (m_pWriter)
	{
		m_pWriter->Flush();
		m_pWriter->Close();
		delete m_pWriter;
		m_pWriter = nullptr;
	}

}


