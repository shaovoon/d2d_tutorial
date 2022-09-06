/*
TextDisplayStatic class to display text with Direct2D

Copyright (c) 2008 Wong Shao Voon

The Code Project Open License (CPOL)
http://www.codeproject.com/info/cpol10.aspx
*/

#pragma once
#include "Common.h"

using namespace D2D1;
using namespace Microsoft::WRL;

// TextDisplayStatic

class TextDisplayStatic : public CStatic
{
	DECLARE_DYNAMIC(TextDisplayStatic)


public:
	TextDisplayStatic();
	virtual ~TextDisplayStatic();

	void SetInfo(const CString& fontFamily, const CString& text, bool centerize, float strokeWidth, float fontSize)
	{
		m_FontFamily = fontFamily;
		m_Text = text;
		m_Centerize = centerize;
		m_StrokeWidth = strokeWidth;
		m_FontSize = fontSize;
	}

private:
	void ClearScreen(ID2D1RenderTarget* target);
	void CreateDeviceResources(ID2D1RenderTarget* target);
	void DrawText(ID2D1RenderTarget* target, const CString& fontname, const CString& text, float fontSize);
	void GetDPI();

private:
	ComPtr<ID2D1DCRenderTarget> m_DCTarget;
	ComPtr<ID2D1SolidColorBrush> m_BlackBrush;
	CString m_FontFamily;
	CString m_Text;
	bool m_Centerize;
	float m_StrokeWidth;
	float m_FontSize;
	float m_DPI;
	float m_DPIScale;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


