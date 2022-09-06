// MyScrollView.cpp : implementation file
//

#include "pch.h"
#include "FontScrollView.h"
#include "../Common/FactorySingleton.h"
#include <algorithm>

// CMyScrollView

IMPLEMENT_DYNCREATE(CFontScrollView, CScrollView)

CFontScrollView::CFontScrollView()
	: m_CellHeight(20)
	, m_FontSize(15.0f)
	, m_SelectedIndex(0)
	, m_DPI(96.0f)
	, m_DPIScale(1.0f)
{

}

CFontScrollView::~CFontScrollView()
{
}

BEGIN_MESSAGE_MAP(CFontScrollView, CScrollView)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()

// CMyScrollView drawing
void CFontScrollView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	GetDPI();

	EnumFont(m_vecFont);

	m_SelectedIndex = 0;

	m_SizeTotal.cx = static_cast<LONG>(180 * m_DPIScale);
	m_SizeTotal.cy = static_cast<LONG>(m_CellHeight * m_vecFont.size() * m_DPIScale);
	SetScrollSizes(MM_TEXT, m_SizeTotal);

}

void CFontScrollView::GetDPI()
{
	m_DPI = static_cast<float>(GetDpiForWindow(GetSafeHwnd()));

	m_DPIScale = m_DPI / USER_DEFAULT_SCREEN_DPI;
}

void CFontScrollView::Render(CDC* pDC)
{
	if (!m_DCTarget)
	{
		RECT rect;
		
		rect.top = 0;
		rect.bottom = m_SizeTotal.cy;
		rect.left = 0;
		rect.right = m_SizeTotal.cx;

		auto size = SizeU(rect.right,
				rect.bottom);

		auto sizeF = SizeF(static_cast<float>(rect.right),
			static_cast<float>(rect.bottom));

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

		m_DCTarget->BindDC(pDC->GetSafeHdc(), &rect);

		HR(m_DCTarget->CreateCompatibleRenderTarget(m_BmpTarget.ReleaseAndGetAddressOf()));

		CreateDeviceResources(m_BmpTarget.Get());

		m_BmpTarget->BeginDraw();
		ClearScreen(m_BmpTarget.Get());
		int i = 0;
		for (auto& value: m_vecFont)
		{
			DrawText(m_BmpTarget.Get(), value->LocalizedName().c_str(), m_FontSize * m_DPIScale, i);
			++i;
		}
		m_BmpTarget->EndDraw();
	}

	ComPtr<ID2D1Bitmap> bitmap;
	m_BmpTarget->GetBitmap(bitmap.GetAddressOf());

	RECT rect;

	rect.top = 0;
	rect.bottom = m_SizeTotal.cy;
	rect.left = 0;
	rect.right = m_SizeTotal.cx;

	//m_DCTarget->SetDpi(m_DPI, m_DPI);
	m_DCTarget->BindDC(pDC->GetSafeHdc(), &rect);
	m_DCTarget->BeginDraw();

	m_DCTarget->DrawBitmap(bitmap.Get());

	if (m_SelectedIndex != -1)
	{
		float top = m_SelectedIndex * m_CellHeight * m_DPIScale;
		auto blue_rect = RectF(0.0f, top, rect.right * m_DPIScale, top + (m_CellHeight * m_DPIScale));
		m_DCTarget->FillRectangle(blue_rect, m_BlueBrush.Get());
	}

	if (D2DERR_RECREATE_TARGET == m_DCTarget->EndDraw())
	{
		m_DCTarget.Reset();
		Invalidate();
	}
}

void CFontScrollView::ClearScreen(ID2D1RenderTarget* target)
{
	target->Clear(COLOR_WHITE);
}

void CFontScrollView::CreateDeviceResources(ID2D1RenderTarget* target)
{
	HR(target->CreateSolidColorBrush(COLOR_BLACK,
		m_BlackBrush.ReleaseAndGetAddressOf()));

	HR(target->CreateSolidColorBrush(COLOR_BLUE,
		m_BlueBrush.ReleaseAndGetAddressOf()));

}

void CFontScrollView::CreateDeviceIndependentResources()
{
}

void CFontScrollView::OnDraw(CDC* pDC)
{
	Render(pDC);
}

// CMyScrollView diagnostics

#ifdef _DEBUG
void CFontScrollView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CFontScrollView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG

BOOL CFontScrollView::OnEraseBkgnd(CDC* pDC)
{
	return CScrollView::OnEraseBkgnd(pDC);
}

void CFontScrollView::EnumFont(std::vector< std::shared_ptr<FontInfo> >& vecFont)
{
	if (vecFont.empty() == false)
		return;

	ComPtr<IDWriteFontCollection> fontCollection;
	HR(FactorySingleton::GetDWriteFactory()->GetSystemFontCollection(
		fontCollection.GetAddressOf(),
		TRUE
	));

	UINT32 familyCount = fontCollection->GetFontFamilyCount();
	for (UINT32 i = 0; i < familyCount; ++i)
	{
		ComPtr<IDWriteFontFamily> fontFamily;
		HR(fontCollection->GetFontFamily(i, fontFamily.GetAddressOf()));
		if (!fontFamily)
			continue;

		ComPtr < IDWriteLocalizedStrings> names;
		HR(fontFamily->GetFamilyNames(names.GetAddressOf()));
		WCHAR wname[100];
		UINT32 localizedCount = names->GetCount();

		std::shared_ptr<FontInfo> info = std::make_shared<FontInfo>();

		HR(names->GetString(localizedCount - 1, wname, _countof(wname)));
		info->LocalizedName(wname);
		HR(names->GetString(0, wname, _countof(wname)));
		info->OriginalName(wname);
		//TRACE(_T("%s\n"), wname);
		UINT32 fontCount = fontFamily->GetFontCount();
		std::vector<FontSubType> vecSubNames;
		vecSubNames.reserve(fontCount);
		for (UINT32 j = 0; j < fontCount; ++j)
		{
			ComPtr<IDWriteFont> font;
			HR(fontFamily->GetFont(j, font.GetAddressOf()));
			if (!font)
				continue;

			ComPtr < IDWriteLocalizedStrings> faceNames;
			font->GetFaceNames(faceNames.GetAddressOf());

			WCHAR wface[100];
			HR(faceNames->GetString(0, wface, _countof(wface)));
			//TRACE(_T("    %s\n"), wface);
			vecSubNames.push_back(FontSubType(wface, font->GetStretch(), font->GetStyle(), font->GetWeight()));
		}
		info->SubTypes(std::move(vecSubNames));
		vecFont.push_back(info);
	}
	
	std::sort(vecFont.begin(), vecFont.end(),
		[](const std::shared_ptr<FontInfo>& a, const std::shared_ptr<FontInfo>& b) 
		{return a->OriginalName() < b->OriginalName(); });
}

bool CFontScrollView::FindFont(const std::wstring& fontName)
{
	ComPtr<IDWriteFontCollection> fontCollection;
	HR(FactorySingleton::GetDWriteFactory()->GetSystemFontCollection(
		fontCollection.GetAddressOf(),
		TRUE
	));

	UINT32 familyCount = fontCollection->GetFontFamilyCount();
	for (UINT32 i = 0; i < familyCount; ++i)
	{
		ComPtr<IDWriteFontFamily> fontFamily;
		HR(fontCollection->GetFontFamily(i, fontFamily.GetAddressOf()));
		if (!fontFamily)
			continue;

		ComPtr < IDWriteLocalizedStrings> names;
		HR(fontFamily->GetFamilyNames(names.GetAddressOf()));
		WCHAR wname[80];

		HR(names->GetString(0, wname, _countof(wname)));
		if (fontName == wname)
			return true;
	}
	return false;
}

void CFontScrollView::DrawText(ID2D1RenderTarget* target, const CString& fontname, float fontSize, int index)
{
	ComPtr<IDWriteTextFormat> textFormat;

	HR(FactorySingleton::GetDWriteFactory()->CreateTextFormat((LPCTSTR)fontname,
		nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL, fontSize, L"",
		textFormat.GetAddressOf()));

	auto size = target->GetSize();

	auto rect = RectF(0.0f, m_CellHeight * index * m_DPIScale, size.width * m_DPIScale, (m_CellHeight * index + size.height) * m_DPIScale);
	target->DrawTextW(fontname, fontname.GetLength(), textFormat.Get(), rect, m_BlackBrush.Get());
}

void CFontScrollView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CPoint pt = GetScrollPosition();

	pt.y += point.y;

	m_SelectedIndex = static_cast<int>(pt.y / (m_CellHeight * m_DPIScale));

	if (m_UpdateDrawFunc)
		m_UpdateDrawFunc();

	Invalidate(FALSE);

	if (!(m_SelectedIndex > -1 && m_SelectedIndex < static_cast<int>(m_vecFont.size())))
		m_SelectedIndex = 0;

	CScrollView::OnLButtonUp(nFlags, point);
}

bool CFontScrollView::SetFontFamily(const CString& family)
{
	for (size_t i = 0; i < m_vecFont.size(); ++i)
	{
		if (family == m_vecFont[i]->OriginalName().c_str())
		{
			m_SelectedIndex = static_cast<int>(i);
			Invalidate(FALSE);
			return true;
		}
	}
	return false;
}

void CFontScrollView::SetSelectionIndex(int index)
{
	m_SelectedIndex = index;
	ScrollToPosition(CPoint(0, static_cast<int>(m_CellHeight * m_DPIScale * m_SelectedIndex)));

	Invalidate(FALSE);
}

void CFontScrollView::ScrollToFont()
{
	ScrollToPosition(CPoint(0, static_cast<int>(m_CellHeight * m_DPIScale * m_SelectedIndex)));
}

int CFontScrollView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	return MA_ACTIVATE;
}