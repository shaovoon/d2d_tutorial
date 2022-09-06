
// D2DNicePhotoDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "D2DNicePhoto.h"
#include "D2DNicePhotoDlg.h"
#include "afxdialogex.h"
#include "../Common/FactorySingleton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CD2DNicePhotoDlg dialog



CD2DNicePhotoDlg::CD2DNicePhotoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_D2DNICEPHOTO_DIALOG, pParent)
	, m_Radius(70)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CD2DNicePhotoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CD2DNicePhotoDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CD2DNicePhotoDlg message handlers

BOOL CD2DNicePhotoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CreateDeviceIndependentResources();
	m_ImageFile = L"yes.jpg";
	m_PatternImageFile = L"GreyWall.png";

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CD2DNicePhotoDlg::OnPaint()
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

			HR(m_Target->CreateCompatibleRenderTarget(m_BmpTarget.ReleaseAndGetAddressOf()));

			CreateDeviceResources();
		}

		if (!(D2D1_WINDOW_STATE_OCCLUDED & m_Target->CheckWindowState()))
		{
			DrawPhoto();
			ComPtr<ID2D1Bitmap> bitmap;
			m_BmpTarget->GetBitmap(bitmap.GetAddressOf());
			
			m_Target->BeginDraw();

			Draw();

			CRect rectClient;
			GetClientRect(&rectClient);
			const auto rectSrc = RectF(0.0f, 0.0f, 276, 276);

			int halfWidth = (rectClient.Width() - 276) / 2;
			int halfHeight = (rectClient.Height() - 276) / 2;
			const auto rectDest = RectF(halfWidth, halfHeight, halfWidth + 276, halfHeight + 276);

			m_Target->DrawBitmap(bitmap.Get(), rectDest, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, rectSrc);


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
HCURSOR CD2DNicePhotoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CD2DNicePhotoDlg::CreateDeviceResources()
{
	HR(m_BmpTarget->CreateSolidColorBrush(ColorF(ColorF::White),
		m_WhiteBrush.ReleaseAndGetAddressOf()));
	HR(m_BmpTarget->CreateSolidColorBrush(ColorF(ColorF::Yellow),
		m_GreyBrush.ReleaseAndGetAddressOf()));
	CreateRadialGradientBrush();
}

void CD2DNicePhotoDlg::CreateDeviceIndependentResources()
{
}

void CD2DNicePhotoDlg::Draw()
{
	DrawImage();
	//DrawImageText();
}

void CD2DNicePhotoDlg::DrawImage()
{
	if (!m_BkgdBitmap)
		LoadImage(m_PatternImageFile, m_Target.Get(), m_BkgdBitmap, m_D2DBkgdBitmap);

	m_Target->Clear(ColorF(ColorF::White));

	if (m_D2DBkgdBitmap)
	{
		auto sizeBmp = m_D2DBkgdBitmap->GetSize();
		//auto rectBmp = RectF(0.0f, 0.0f, sizeBmp.width, sizeBmp.height);
		CRect rectClient;
		GetClientRect(&rectClient);
		auto rectBmp = RectF(0.0f, 0.0f, rectClient.Width(), rectClient.Height());

		m_Target->DrawBitmap(m_D2DBkgdBitmap.Get(), rectBmp);
	}
}

void CD2DNicePhotoDlg::LoadImage(LPCTSTR file, ID2D1RenderTarget* rt, ComPtr<IWICFormatConverter>& bmp, ComPtr<ID2D1Bitmap>& d2dBmp)
{
	CString sFile = file;

	if (sFile == L"")
		return;

	ComPtr<IWICBitmapDecoder> decoder;
	HR(FactorySingleton::GetImageFactory()->CreateDecoderFromFilename(file, nullptr, GENERIC_READ,
		WICDecodeMetadataCacheOnLoad, decoder.GetAddressOf()));

	ComPtr<IWICBitmapFrameDecode> source;
	HR(decoder->GetFrame(0, source.GetAddressOf()));

	HR(FactorySingleton::GetImageFactory()->CreateFormatConverter(bmp.ReleaseAndGetAddressOf()));

	HR(bmp->Initialize(source.Get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone,
		nullptr, 0.0, WICBitmapPaletteTypeMedianCut));

	HR(rt->CreateBitmapFromWicBitmap(bmp.Get(), d2dBmp.ReleaseAndGetAddressOf()));
}

void CD2DNicePhotoDlg::DrawPhoto()
{
	if (!m_PhotoBitmap)
		LoadImage(m_ImageFile, m_BmpTarget.Get(), m_PhotoBitmap, m_D2DPhotoBitmap);

	m_BmpTarget->BeginDraw();
	m_BmpTarget->Clear(ColorF(0.26f, 0.56f, 0.87f));
	const auto rect = RectF(0.0f, 0.0f, 276, 276);
	m_BmpTarget->FillRectangle(&rect, m_WhiteBrush.Get());

	const D2D1_ELLIPSE ellTopLeft = Ellipse(Point2F(0.0f, 0.0f), m_Radius, m_Radius);
	m_BmpTarget->FillEllipse(
		ellTopLeft,
		m_RadialBrushTopLeft.Get());
	const D2D1_ELLIPSE ellBottomLeft = Ellipse(Point2F(0.0f, 276.0f), m_Radius, m_Radius);
	m_BmpTarget->FillEllipse(
		ellBottomLeft,
		m_RadialBrushBottomLeft.Get());
	const D2D1_ELLIPSE ellBottomRight = Ellipse(Point2F(276.0f, 276.0f), m_Radius, m_Radius);
	m_BmpTarget->FillEllipse(
		ellBottomRight,
		m_RadialBrushBottomRight.Get());
	const D2D1_ELLIPSE ellTopRight = Ellipse(Point2F(276.0f, 0.0f), m_Radius, m_Radius);
	m_BmpTarget->FillEllipse(
		ellTopRight,
		m_RadialBrushTopRight.Get());

	const auto rectSrc = RectF(0.0f, 0.0f, 256, 256);

	const auto rectDest = RectF(10.0f, 10.0f, 266, 266);

	m_BmpTarget->DrawBitmap(m_D2DPhotoBitmap.Get(), rectDest, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, rectSrc);
	m_BmpTarget->EndDraw();
}

void CD2DNicePhotoDlg::CreateRadialGradientBrush()
{
	float gray = 0.66f;
	float blue = 0.72f;
	D2D1_GRADIENT_STOP stops[] =
	{
		{ 0.0f, ColorF(gray,gray,blue) },
		{ 1.0f, ColorF(ColorF::White) }
	};

	ComPtr<ID2D1GradientStopCollection> collection;
	HR(m_BmpTarget->CreateGradientStopCollection(stops, _countof(stops), collection.GetAddressOf()));

	D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES props = {};
	props.center = Point2F(0.0f, 0.0f);
	//props.gradientOriginOffset = Point2F(100.0f, 100.0f);
	HR(m_BmpTarget->CreateRadialGradientBrush(props, collection.Get(), m_RadialBrushTopLeft.ReleaseAndGetAddressOf()));


	props.center = Point2F(0.0f, 276.0f);
	HR(m_BmpTarget->CreateRadialGradientBrush(props, collection.Get(), m_RadialBrushBottomLeft.ReleaseAndGetAddressOf()));


	props.center = Point2F(276.0f, 276.0f);
	HR(m_BmpTarget->CreateRadialGradientBrush(props, collection.Get(), m_RadialBrushBottomRight.ReleaseAndGetAddressOf()));


	props.center = Point2F(276.0f, 0.0f);
	HR(m_BmpTarget->CreateRadialGradientBrush(props, collection.Get(), m_RadialBrushTopRight.ReleaseAndGetAddressOf()));

	m_RadialBrushTopLeft->SetRadiusX(m_Radius);
	m_RadialBrushTopLeft->SetRadiusY(m_Radius);

	m_RadialBrushBottomLeft->SetRadiusX(m_Radius);
	m_RadialBrushBottomLeft->SetRadiusY(m_Radius);

	m_RadialBrushBottomRight->SetRadiusX(m_Radius);
	m_RadialBrushBottomRight->SetRadiusY(m_Radius);

	m_RadialBrushTopRight->SetRadiusX(m_Radius);
	m_RadialBrushTopRight->SetRadiusY(m_Radius);
}
