#pragma once
#include <wrl.h>
#include <vector>
#include "../Common/ComException.h"
#include <assert.h>

using namespace D2D1;
using namespace Microsoft::WRL;

//colors
D2D_COLOR_F const COLOR_WHITE = { 1.0f,  1.0f,  1.0f,  1.0f };
D2D_COLOR_F const COLOR_BLACK = { 0.0f,  0.0f,  0.0f,  1.0f };
D2D_COLOR_F const COLOR_BLUE = { 0.0f,  0.0f,  1.0f,  0.5f };

template <typename T>
void CreateInstance(REFCLSID clsid, Microsoft::WRL::ComPtr<T>& ptr)
{
	assert(!ptr);
	HR(CoCreateInstance(clsid, nullptr, CLSCTX_INPROC_SERVER,
		__uuidof(T), reinterpret_cast<void**>(ptr.GetAddressOf())));
}

struct FontSubType
{
	FontSubType() 
		: m_Stretch(DWRITE_FONT_STRETCH_NORMAL)
		, m_Style(DWRITE_FONT_STYLE_NORMAL)
		, m_Weight(DWRITE_FONT_WEIGHT_NORMAL)
	{}
	FontSubType(const std::wstring& subName, DWRITE_FONT_STRETCH stretch, DWRITE_FONT_STYLE style, DWRITE_FONT_WEIGHT weight)
		: m_SubName(subName)
		, m_Stretch(stretch)
		, m_Style(style)
		, m_Weight(weight)
	{}
	FontSubType(FontSubType&& o) noexcept
		: m_SubName(std::move(o.m_SubName))
		, m_Stretch(o.m_Stretch)
		, m_Style(o.m_Style)
		, m_Weight(o.m_Weight)
	{}
	FontSubType(const FontSubType& o)
		: m_SubName(o.m_SubName)
		, m_Stretch(o.m_Stretch)
		, m_Style(o.m_Style)
		, m_Weight(o.m_Weight)
	{}
	FontSubType& operator=(const FontSubType& o)
	{
		if (this == &o)
			return *this;
		
		m_SubName = o.m_SubName;
		m_Stretch = o.m_Stretch;
		m_Style = o.m_Style;
		m_Weight = o.m_Weight;

		return *this;
	}

	const std::wstring& SubName() const { return m_SubName; }
	void SubName(const std::wstring& val) { m_SubName = val; }

	DWRITE_FONT_STRETCH Stretch() const { return m_Stretch; }
	void Stretch(DWRITE_FONT_STRETCH val) { m_Stretch = val; }

	DWRITE_FONT_STYLE Style() const { return m_Style; }
	void Style(DWRITE_FONT_STYLE val) { m_Style = val; }

	DWRITE_FONT_WEIGHT Weight() const { return m_Weight; }
	void Weight(DWRITE_FONT_WEIGHT val) { m_Weight = val; }

private:
	std::wstring m_SubName;
	DWRITE_FONT_STRETCH m_Stretch;
	DWRITE_FONT_STYLE m_Style;
	DWRITE_FONT_WEIGHT m_Weight;
};

struct FontInfo
{
public:
	FontInfo() : m_StartY(0), m_Height(0)
	{}
	FontInfo(FontInfo&& o) noexcept
		: m_OriginalName(std::move(o.m_OriginalName))
		, m_LocalizedName(std::move(o.m_LocalizedName))
		, m_SubTypes(std::move(o.m_SubTypes))
		, m_StartY(o.m_StartY)
		, m_Height(o.m_Height)
	{}
	FontInfo(const FontInfo& o)
		: m_OriginalName(o.m_OriginalName)
		, m_LocalizedName(o.m_LocalizedName)
		, m_SubTypes(o.m_SubTypes)
		, m_StartY(o.m_StartY)
		, m_Height(o.m_Height)
	{}
	FontInfo& operator=(const FontInfo& o)
	{
		if (this == &o)
			return *this;

		m_OriginalName = o.m_OriginalName;
		m_LocalizedName = o.m_LocalizedName;
		m_SubTypes = o.m_SubTypes;
		m_StartY = o.m_StartY;
		m_Height = o.m_Height;

		return *this;
	}
	const std::wstring& OriginalName() const { return m_OriginalName; }
	void OriginalName(const std::wstring& val) { m_OriginalName = val; }
	void OriginalName(const WCHAR* val) { m_OriginalName = val; }
	const std::wstring& LocalizedName() const { return m_LocalizedName; }
	void LocalizedName(const std::wstring& val) { m_LocalizedName = val; }
	void LocalizedName(const WCHAR* val) { m_LocalizedName = val; }
	const std::vector<FontSubType>& SubTypes() const { return m_SubTypes; }
	void SubTypes(std::vector<FontSubType>&& val) { m_SubTypes = std::move(val); }
	UINT32 StartY() const { return m_StartY; }
	void StartY(UINT32 val) { m_StartY = val; }
	UINT32 Height() const { return m_Height; }
	void Height(UINT32 val) { m_Height = val; }

private:
	std::wstring m_OriginalName;
	std::wstring m_LocalizedName;
	std::vector<FontSubType> m_SubTypes;
	UINT32 m_StartY;
	UINT32 m_Height;
};
