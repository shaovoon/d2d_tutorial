#pragma once

// CFontScrollView view
#include "Common.h"
#include <functional>
#include <vector>
#include <memory>

using namespace D2D1;
using namespace Microsoft::WRL;

class CFontScrollView : public CScrollView
{
	DECLARE_DYNCREATE(CFontScrollView)

public:
	static void EnumFont(std::vector< std::shared_ptr<FontInfo> >& vecFont);
	static bool FindFont(const std::wstring& fontName);

	void Render(CDC* pDC);
	void ClearScreen(ID2D1RenderTarget* target);
	void CreateDeviceResources(ID2D1RenderTarget* target);
	void CreateDeviceIndependentResources();
	void DrawText(ID2D1RenderTarget* target, const CString& fontname, float fontSize, int index);
	const CString GetFontFamily() const { return m_vecFont.at(m_SelectedIndex)->OriginalName().c_str(); }
	bool SetFontFamily(const CString& family);
	int GetSelectionIndex() const { return m_SelectedIndex; }
	void SetSelectionIndex(int index); // nothing to record for undo/redo.

	void ScrollToFont();
	void SetUpdateDrawFunc(std::function<void()> func)
	{
		m_UpdateDrawFunc = func;
	}

	int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);


private:
	ComPtr<ID2D1DCRenderTarget> m_DCTarget;
	ComPtr<ID2D1BitmapRenderTarget> m_BmpTarget;
	ComPtr<ID2D1SolidColorBrush> m_BlackBrush;
	ComPtr<ID2D1SolidColorBrush> m_BlueBrush;

	CSize m_SizeTotal;
	int m_CellHeight;
	float m_FontSize;
	int m_SelectedIndex;
	std::vector< std::shared_ptr<FontInfo> > m_vecFont;
	std::function<void()> m_UpdateDrawFunc;
	float m_DPI;
	float m_DPIScale;

	void GetDPI();

protected:
	CFontScrollView();           // protected constructor used by dynamic creation
	virtual ~CFontScrollView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


