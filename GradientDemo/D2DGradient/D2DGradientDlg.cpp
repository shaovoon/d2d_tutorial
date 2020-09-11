
// D2DGradientDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "D2DGradient.h"
#include "D2DGradientDlg.h"
#include "afxdialogex.h"
#include "../Common/FactorySingleton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CD2DGradientDlg dialog



CD2DGradientDlg::CD2DGradientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_D2DGRADIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CD2DGradientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CD2DGradientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CD2DGradientDlg message handlers

BOOL CD2DGradientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_Text = L"ROCKSTAR DEVELOPER!";
	CreateDeviceIndependentResources();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CD2DGradientDlg::OnPaint()
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
HCURSOR CD2DGradientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CD2DGradientDlg::CreateDeviceResources()
{
	CreateLinearGradientBrush();
	CreateRadialGradientBrush();
}

void CD2DGradientDlg::CreateLinearGradientBrush()
{
	/*
	D2D1_GRADIENT_STOP stops[] =
	{
		{ 0.0f, ColorF(ColorF::Cyan) },
		{ 1.0f, ColorF(ColorF::DarkBlue) }
	};
	*/
	D2D1_GRADIENT_STOP stops[] =
	{
		{ 0.0f, ColorF(227.0f / 255.0f, 9.0f / 255.0f, 64.0f / 255.0f, 1.0f) },
		{ 0.33f, ColorF(231.0f / 255.0f, 215.0f / 255.0f, 2.0f / 255.0f, 1.0f) },
		{ 0.66f, ColorF(15.0f / 255.0f, 168.0f / 255.0f, 149.0f / 255.0f, 1.0f) },
		{ 1.0f, ColorF(19.0f / 255.0f, 115.0f / 255.0f, 232.0f / 255.0f, 1.0f) }
	};
	ComPtr<ID2D1GradientStopCollection> collection;

	HR(m_Target->CreateGradientStopCollection(stops, _countof(stops),
		collection.GetAddressOf()));

	D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES props = {};

	HR(m_Target->CreateLinearGradientBrush(props, collection.Get(),
		m_LinearBrush.ReleaseAndGetAddressOf()));
}

void CD2DGradientDlg::CreateRadialGradientBrush()
{
	D2D1_GRADIENT_STOP stops[] =
	{
		{ 0.0f, ColorF(ColorF::Cyan) },
		{ 1.0f, ColorF(ColorF::DarkBlue) }
	};

	ComPtr<ID2D1GradientStopCollection> collection;
	HR(m_Target->CreateGradientStopCollection(stops, _countof(stops), collection.GetAddressOf()));

	D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES props = {};
	props.center = Point2F(120.0f, 80.0f);
	//props.gradientOriginOffset = Point2F(100.0f, 100.0f);
	HR(m_Target->CreateRadialGradientBrush(props, collection.Get(), m_RadialBrush.ReleaseAndGetAddressOf()));
}

void CD2DGradientDlg::CreateDeviceIndependentResources()
{
	HR(FactorySingleton::GetDWriteFactory()->CreateTextFormat(L"Arial Black",
		nullptr, DWRITE_FONT_WEIGHT_ULTRA_BOLD, DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL, 40, L"",
		m_TextFormat.ReleaseAndGetAddressOf()));
}

void CD2DGradientDlg::Draw()
{
	m_Target->Clear(ColorF(ColorF::White));

	//DrawLinearGradientRect();
	//DrawLinearGradientText();
	//DrawRadialGradientRect();
	DrawRadialGradientText();
}

void CD2DGradientDlg::DrawLinearGradientRect()
{
	auto size = m_Target->GetSize();

	m_LinearBrush->SetStartPoint(Point2F(0.0f, 0.0f));

	m_LinearBrush->SetEndPoint(Point2F(size.width, 0.0f));

	auto r = RectF(0.0f, 0.0f, size.width, size.height);

	m_Target->FillRectangle(r, m_LinearBrush.Get());
}

void CD2DGradientDlg::DrawLinearGradientText()
{
	auto size = m_Target->GetSize();

	m_LinearBrush->SetStartPoint(Point2F(0.0f, 0.0f));

	m_LinearBrush->SetEndPoint(Point2F(size.width, 0.0f));

	auto r = RectF(0.0f, 0.0f, size.width, size.height);

	m_Target->DrawTextW((LPCTSTR)m_Text, m_Text.GetLength(), m_TextFormat.Get(), &r, m_LinearBrush.Get());
}

void CD2DGradientDlg::DrawRadialGradientRect()
{
	auto size = m_Target->GetSize();
	auto radius = min(size.width, size.height);

	m_RadialBrush->SetRadiusX(radius);
	m_RadialBrush->SetRadiusY(radius);

	m_Target->FillRectangle(RectF(0.0f, 0.0f, size.width, size.height), m_RadialBrush.Get());
}

void CD2DGradientDlg::DrawRadialGradientText()
{
	auto size = m_Target->GetSize();
	auto radius = min(size.width, size.height);

	m_RadialBrush->SetRadiusX(radius);
	m_RadialBrush->SetRadiusY(radius);

	auto r = RectF(0.0f, 0.0f, size.width, size.height);

	m_Target->DrawTextW((LPCTSTR)m_Text, m_Text.GetLength(), m_TextFormat.Get(), &r, m_RadialBrush.Get());
}
