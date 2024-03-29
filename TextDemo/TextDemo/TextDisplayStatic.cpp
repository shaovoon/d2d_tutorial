/*
TextDisplayStatic class to display text with Direct2D

Copyright (c) 2022 Wong Shao Voon

The Code Project Open License (CPOL)
http://www.codeproject.com/info/cpol10.aspx
*/

// TextDisplayStatic.cpp : implementation file
//

#include "pch.h"
#include "TextDisplayStatic.h"
#include "../Common/FactorySingleton.h"
#include "../Common/ComException.h"

// TextDisplayStatic

IMPLEMENT_DYNAMIC(TextDisplayStatic, CStatic)

TextDisplayStatic::TextDisplayStatic()
	: m_Centerize(true)
	, m_StrokeWidth(1.0f)
	, m_FontSize(26.0f)
	, m_DPI(96.0f)
	, m_DPIScale(1.0f)
	, m_Italic(false)
	, m_Bold(false)
{

}

TextDisplayStatic::~TextDisplayStatic()
{
}


BEGIN_MESSAGE_MAP(TextDisplayStatic, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// TextDisplayStatic message handlers
void TextDisplayStatic::OnPaint()
{
	GetDPI();

	CPaintDC dc(this); 

	CRect rect2;
	GetClientRect(rect2);

	RECT rect;

	rect.top = 0;
	rect.bottom = rect2.Height();
	rect.left = 0;
	rect.right = rect2.Width();

	if (!m_DCTarget)
	{
		// Create a pixel format and initial its format
		// and alphaMode fields.
		// https://docs.microsoft.com/en-gb/windows/win32/direct2d/supported-pixel-formats-and-alpha-modes#supported-formats-for-id2d1devicecontext
		D2D1_PIXEL_FORMAT pixelFormat = D2D1::PixelFormat(
			DXGI_FORMAT_B8G8R8A8_UNORM,
			D2D1_ALPHA_MODE_PREMULTIPLIED
		);

		D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties();
		props.pixelFormat = pixelFormat;

		HR(FactorySingleton::GetGraphicsFactory()->CreateDCRenderTarget(&props,
			m_DCTarget.GetAddressOf()));

		CreateDeviceResources(m_DCTarget.Get());
	}

	m_DCTarget->BindDC(dc.GetSafeHdc(), &rect);
	m_DCTarget->BeginDraw();

	ClearScreen(m_DCTarget.Get());

	auto size = m_DCTarget->GetSize();

	auto rectf = RectF(0.0f, 0.0f, size.width, size.height);

	m_DCTarget->DrawRectangle(rectf, m_BlackBrush.Get(), m_StrokeWidth);

	ComPtr<IDWriteTextFormat> textFormat = GetTextFormat(m_FontFamily, m_FontSize * m_DPIScale,
		m_Italic, m_Bold, m_Centerize, m_Centerize);

	DrawText(m_DCTarget.Get(), textFormat.Get(), m_Text);

	if (D2DERR_RECREATE_TARGET == m_DCTarget->EndDraw())
	{
		m_DCTarget.Reset();
		Invalidate();
	}
}

void TextDisplayStatic::ClearScreen(ID2D1RenderTarget* target)
{
	target->Clear(COLOR_WHITE);
}

void TextDisplayStatic::CreateDeviceResources(ID2D1RenderTarget* target)
{
	HR(target->CreateSolidColorBrush(COLOR_BLACK,
		m_BlackBrush.ReleaseAndGetAddressOf()));
}

void TextDisplayStatic::DrawText(ID2D1RenderTarget* target, IDWriteTextFormat* textFormat, const CString& text)
{
	auto size = target->GetSize();

	auto rect = RectF(0.0f, 0.0f, size.width, size.height);
	target->DrawTextW(text, text.GetLength(), textFormat, rect, m_BlackBrush.Get());
}

ComPtr<IDWriteTextFormat> TextDisplayStatic::GetTextFormat(const CString& fontname, float fontSize,
	bool italic, bool bold, bool centerHorizontal, bool centerVertical)
{
	ComPtr<IDWriteTextFormat> textFormat;

	DWRITE_FONT_STYLE fontStyle = italic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL;
	DWRITE_FONT_WEIGHT fontWeight = bold ? DWRITE_FONT_WEIGHT_BOLD : DWRITE_FONT_WEIGHT_NORMAL;

	HR(FactorySingleton::GetDWriteFactory()->CreateTextFormat((LPCTSTR)fontname,
		nullptr, fontWeight, fontStyle,
		DWRITE_FONT_STRETCH_NORMAL, fontSize, L"",
		textFormat.GetAddressOf()));

	if (centerHorizontal)
		textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

	if (centerVertical)
		textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	return textFormat;
}

void TextDisplayStatic::GetDPI()
{
	m_DPI = static_cast<float>(GetDpiForWindow(GetSafeHwnd()));

	m_DPIScale = m_DPI / USER_DEFAULT_SCREEN_DPI;
}
