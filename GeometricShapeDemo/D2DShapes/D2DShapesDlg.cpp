
// D2DShapesDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "D2DShapes.h"
#include "D2DShapesDlg.h"
#include "afxdialogex.h"
#include "../Common/FactorySingleton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CD2DShapesDlg dialog
template <class T> void SafeRelease(T** ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}


CD2DShapesDlg::CD2DShapesDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_D2DSHAPES_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CD2DShapesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CD2DShapesDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CD2DShapesDlg message handlers

BOOL CD2DShapesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CD2DShapesDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();

		if (!m_Target)
		{
			CRect rc;
			GetClientRect(rc);

			D2D1_SIZE_U size = D2D1::SizeU(
				rc.right - rc.left,
				rc.bottom - rc.top
			);

			HR(FactorySingleton::GetGraphicsFactory()->CreateHwndRenderTarget(
				RenderTargetProperties(),
				HwndRenderTargetProperties(GetSafeHwnd(), size),
				m_Target.ReleaseAndGetAddressOf()));

			CreateDeviceResources();
		}

		if (!(D2D1_WINDOW_STATE_OCCLUDED & m_Target->CheckWindowState()))
		{
			m_Target->BeginDraw();

			Draw();

			if (D2DERR_RECREATE_TARGET == m_Target->EndDraw())
			{
				m_Target.Reset();
				Invalidate();
			}
		}
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CD2DShapesDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CD2DShapesDlg::CreateDeviceResources()
{
	HR(m_Target->CreateSolidColorBrush(ColorF(ColorF::Crimson),
		m_StrokeBrush.ReleaseAndGetAddressOf()));
	HR(m_Target->CreateSolidColorBrush(ColorF(ColorF::Yellow),
		m_FillBrush.ReleaseAndGetAddressOf()));
}

void CD2DShapesDlg::CreateDeviceIndependentResources()
{
}

void CD2DShapesDlg::Draw()
{
	m_Target->Clear(ColorF(ColorF::White));
	DrawLine();
	//DrawRectangle();
	//DrawRoundedRectangle();
	//DrawCircle();
	//DrawTriangle();
}
/*
ComPtr<ID2D1StrokeStyle> CD2DShapesDlg::CreateStrokeStyle()
{
	ComPtr<ID2D1StrokeStyle> strokeStyle;

	HR(FactorySingleton::GetGraphicsFactory()->CreateStrokeStyle(
		D2D1::StrokeStyleProperties(
			D2D1_CAP_STYLE_SQUARE,
			D2D1_CAP_STYLE_SQUARE,
			D2D1_CAP_STYLE_FLAT,
			D2D1_LINE_JOIN_MITER,
			0.0f,
			D2D1_DASH_STYLE_SOLID,
			0.0f),
		nullptr,
		0,
		strokeStyle.GetAddressOf()
	));

	return strokeStyle;
}
*/

ComPtr<ID2D1StrokeStyle> CD2DShapesDlg::CreateStrokeStyle()
{
	ComPtr<ID2D1StrokeStyle> strokeStyle;

	HR(FactorySingleton::GetGraphicsFactory()->CreateStrokeStyle(
		D2D1::StrokeStyleProperties(
			D2D1_CAP_STYLE_ROUND,
			D2D1_CAP_STYLE_ROUND,
			D2D1_CAP_STYLE_ROUND,
			D2D1_LINE_JOIN_ROUND,
			0.0f,
			D2D1_DASH_STYLE_SOLID,
			0.0f),
		nullptr,
		0,
		strokeStyle.GetAddressOf()
	));

	return strokeStyle;
}


void CD2DShapesDlg::DrawLine()
{
	m_StrokeBrush->SetColor(ColorF(ColorF::Black));

	ComPtr<ID2D1StrokeStyle> stroke = CreateStrokeStyle();
	m_Target->DrawLine(
		Point2F(10.0f, 40.0f),
		Point2F(110.0f, 40.0f),
		m_StrokeBrush.Get(),
		8.0f,
		stroke.Get());
}

void CD2DShapesDlg::DrawRectangle()
{
	ComPtr<ID2D1StrokeStyle> stroke = CreateStrokeStyle();
	const D2D1_RECT_F rect = RectF(10, 10, 210, 160);
	m_Target->DrawRectangle(
		rect,
		m_StrokeBrush.Get(),
		16.0f,
		stroke.Get());

	m_Target->FillRectangle(
		rect,
		m_FillBrush.Get());
}

void CD2DShapesDlg::DrawRoundedRectangle()
{
	ComPtr<ID2D1StrokeStyle> stroke = CreateStrokeStyle();
	const D2D1_RECT_F rect = RectF(10, 10, 210, 160);

	const D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(
		rect,
		20.f,
		20.f
	);
	
	m_Target->DrawRoundedRectangle(
		roundedRect,
		m_StrokeBrush.Get(),
		16.0f,
		stroke.Get());

	m_Target->FillRoundedRectangle(
		roundedRect,
		m_FillBrush.Get());
}

void CD2DShapesDlg::DrawCircle()
{
	ComPtr<ID2D1StrokeStyle> stroke = CreateStrokeStyle();
	const D2D1_ELLIPSE ell = Ellipse(Point2F(100.0f, 100.0f), 90, 90);
	m_Target->DrawEllipse(
		ell,
		m_StrokeBrush.Get(),
		16.0f,
		stroke.Get());

	m_Target->FillEllipse(
		ell,
		m_FillBrush.Get());
}

ComPtr<ID2D1PathGeometry> CD2DShapesDlg::GenTriangleGeometry(D2D1_POINT_2F pt1, D2D1_POINT_2F pt2, D2D1_POINT_2F pt3)
{
	ID2D1GeometrySink* pSink = NULL;
	HRESULT hr = S_OK;
	ComPtr<ID2D1PathGeometry> m_pPathGeometry;
	// Create a path geometry.
	if (SUCCEEDED(hr))
	{
		hr = FactorySingleton::GetGraphicsFactory()->CreatePathGeometry(m_pPathGeometry.ReleaseAndGetAddressOf());

		if (SUCCEEDED(hr))
		{
			// Write to the path geometry using the geometry sink.
			hr = m_pPathGeometry->Open(&pSink);

			if (SUCCEEDED(hr))
			{
				pSink->BeginFigure(
					pt1,
					D2D1_FIGURE_BEGIN_FILLED
				);

				pSink->AddLine(pt2);


				pSink->AddLine(pt3);

				pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

				hr = pSink->Close();
			}
			SafeRelease(&pSink);
		}
	}
	return m_pPathGeometry;
}

void CD2DShapesDlg::DrawTriangle()
{
	ComPtr<ID2D1StrokeStyle> stroke = CreateStrokeStyle();
	ComPtr<ID2D1PathGeometry> geometry = GenTriangleGeometry(
		Point2F(110, 10), Point2F(210, 140), Point2F(10, 140));
	m_Target->DrawGeometry(
		geometry.Get(),
		m_StrokeBrush.Get(),
		16.0f,
		stroke.Get());

	m_Target->FillGeometry(
		geometry.Get(),
		m_FillBrush.Get());
}
