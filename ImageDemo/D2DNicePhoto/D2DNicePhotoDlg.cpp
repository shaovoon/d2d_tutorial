
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
	, m_Margin(10.0f)
	, m_ImgWidth(0.0f)
	, m_ImgHeight(0.0f)
	, m_FullImgWidth(0.0f)
	, m_FullImgHeight(0.0f)
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
			const auto rectSrc = RectF(0.0f, 0.0f, m_FullImgWidth, m_FullImgHeight);

			int halfWidth = (rectClient.Width() - m_FullImgWidth) / 2;
			int halfHeight = (rectClient.Height() - m_FullImgHeight) / 2;
			const auto rectDest = RectF(halfWidth, halfHeight, halfWidth + m_FullImgWidth, halfHeight + m_FullImgHeight);

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
}

void CD2DNicePhotoDlg::CreateDeviceIndependentResources()
{
}

void CD2DNicePhotoDlg::Draw()
{
	DrawImage();
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

	UINT width=0;
	UINT height = 0;
	m_PhotoBitmap->GetSize(&width, &height);

	m_ImgWidth = width;
	m_ImgHeight = height;
	m_FullImgWidth = m_ImgWidth + m_Margin * 2.0f; // with margins
	m_FullImgHeight = m_ImgHeight + m_Margin * 2.0f;  // with margins

	CreateRadialGradientBrush();

	m_BmpTarget->BeginDraw();
	m_BmpTarget->Clear(ColorF(0.26f, 0.56f, 0.87f));
	const auto rect = RectF(0.0f, 0.0f, m_FullImgWidth, m_FullImgHeight);
	m_BmpTarget->FillRectangle(&rect, m_WhiteBrush.Get());

	const auto rectSrc = RectF(0.0f, 0.0f, m_ImgWidth, m_ImgHeight);

	const auto rectDest = RectF(m_Margin, m_Margin, m_ImgWidth + m_Margin, m_ImgHeight + m_Margin);

	m_BmpTarget->DrawBitmap(m_D2DPhotoBitmap.Get(), rectDest, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, rectSrc);

	const D2D1_ELLIPSE ellTopLeft = Ellipse(Point2F(0.0f, 0.0f), m_Radius, m_Radius);
	m_BmpTarget->FillEllipse(
		ellTopLeft,
		m_RadialBrushTopLeft.Get());
	const D2D1_ELLIPSE ellBottomLeft = Ellipse(Point2F(0.0f, m_FullImgHeight), m_Radius, m_Radius);
	m_BmpTarget->FillEllipse(
		ellBottomLeft,
		m_RadialBrushBottomLeft.Get());
	const D2D1_ELLIPSE ellBottomRight = Ellipse(Point2F(m_FullImgWidth, m_FullImgHeight), m_Radius, m_Radius);
	m_BmpTarget->FillEllipse(
		ellBottomRight,
		m_RadialBrushBottomRight.Get());
	const D2D1_ELLIPSE ellTopRight = Ellipse(Point2F(m_FullImgWidth, 0.0f), m_Radius, m_Radius);
	m_BmpTarget->FillEllipse(
		ellTopRight,
		m_RadialBrushTopRight.Get());

	m_BmpTarget->EndDraw();
}

void CD2DNicePhotoDlg::CreateRadialGradientBrush()
{
	float gray = 0.50f;
	float blue = 0.60f;
	D2D1_GRADIENT_STOP stops[] =
	{
		{ 0.0f, ColorF(gray,gray,blue,0.7f) },
		{ 1.0f, ColorF(1.0f,1.0f,1.0f,0.0f) }
	};

	ComPtr<ID2D1GradientStopCollection> collection;
	HR(m_BmpTarget->CreateGradientStopCollection(stops, _countof(stops), collection.GetAddressOf()));

	D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES props = {};
	props.center = Point2F(0.0f, 0.0f);
	//props.gradientOriginOffset = Point2F(100.0f, 100.0f);
	HR(m_BmpTarget->CreateRadialGradientBrush(props, collection.Get(), m_RadialBrushTopLeft.ReleaseAndGetAddressOf()));


	props.center = Point2F(0.0f, m_FullImgHeight);
	HR(m_BmpTarget->CreateRadialGradientBrush(props, collection.Get(), m_RadialBrushBottomLeft.ReleaseAndGetAddressOf()));


	props.center = Point2F(m_FullImgWidth, m_FullImgHeight);
	HR(m_BmpTarget->CreateRadialGradientBrush(props, collection.Get(), m_RadialBrushBottomRight.ReleaseAndGetAddressOf()));


	props.center = Point2F(m_FullImgWidth, 0.0f);
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
