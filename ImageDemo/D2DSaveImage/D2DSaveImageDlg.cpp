
// D2DSaveImageDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "D2DSaveImage.h"
#include "D2DSaveImageDlg.h"
#include "afxdialogex.h"
#include "../Common/FactorySingleton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CD2DSaveImageDlg dialog



CD2DSaveImageDlg::CD2DSaveImageDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_D2DSAVEIMAGE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CD2DSaveImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CD2DSaveImageDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CD2DSaveImageDlg message handlers

BOOL CD2DSaveImageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CreateDeviceIndependentResources();
	m_PatternImageFile = L"GreyWall.png";
	m_Text = L"SAVE\nPLANET\nEARTH!";

	PaintAndSaveImage(L"C:\\temp\\sample.PNG");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CD2DSaveImageDlg::OnPaint()
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
	}
}



// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CD2DSaveImageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CD2DSaveImageDlg::PaintAndSaveImage(PCWSTR filename)
{
	if (!m_Target)
	{
		// Create a pixel format and initial its format
		// and alphaMode fields.
		// https://docs.microsoft.com/en-gb/windows/win32/direct2d/supported-pixel-formats-and-alpha-modes#supported-formats-for-id2d1devicecontext
		D2D1_PIXEL_FORMAT pixelFormat = D2D1::PixelFormat(
			DXGI_FORMAT_B8G8R8A8_UNORM,
			D2D1_ALPHA_MODE_PREMULTIPLIED
		);

		CRect rc;
		GetClientRect(rc);

		D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties();
		props.pixelFormat = pixelFormat;

		HR(FactorySingleton::GetImageFactory()->CreateBitmap(rc.right, rc.bottom,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapCacheOnLoad,
			m_WicBitmap.ReleaseAndGetAddressOf()));

		HR(FactorySingleton::GetGraphicsFactory()->CreateWicBitmapRenderTarget(m_WicBitmap.Get(),
			RenderTargetProperties(), m_Target.ReleaseAndGetAddressOf()));

		CreateDeviceResources();
	}

	m_Target->BeginDraw();

	Draw();

	if (D2DERR_RECREATE_TARGET == m_Target->EndDraw())
	{
		m_Target.Reset();
		Invalidate();
	}
	else
	{
		SaveAs(m_WicBitmap, filename);
	}
}


void CD2DSaveImageDlg::CreateDeviceResources()
{
	HR(m_Target->CreateSolidColorBrush(ColorF(ColorF::AntiqueWhite),
		m_FillBrush.ReleaseAndGetAddressOf()));
}

void CD2DSaveImageDlg::CreateDeviceIndependentResources()
{
	HR(FactorySingleton::GetDWriteFactory()->CreateTextFormat(L"Arial Black",
		nullptr, DWRITE_FONT_WEIGHT_ULTRA_BOLD, DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL, 80, L"",
		m_TextFormat.ReleaseAndGetAddressOf()));
}

void CD2DSaveImageDlg::Draw()
{
	DrawImage();
}

void CD2DSaveImageDlg::DrawImage()
{
	if (!m_BkgdBitmap)
		LoadImage(m_PatternImageFile);

	m_Target->Clear(ColorF(ColorF::White));

	if (m_D2DBkgdBitmap)
	{
		auto sizeBmp = m_D2DBkgdBitmap->GetSize();
		auto rectBmp = RectF(0.0f, 0.0f, sizeBmp.width, sizeBmp.height);

		m_Target->DrawBitmap(m_D2DBkgdBitmap.Get(), rectBmp);
		m_Target->DrawTextW((LPCTSTR)m_Text, m_Text.GetLength(), m_TextFormat.Get(), &rectBmp, m_FillBrush.Get());

	}
}

void CD2DSaveImageDlg::LoadImage(LPCTSTR file)
{
	CString sFile = file;

	if (sFile == L"")
		return;

	ComPtr<IWICBitmapDecoder> decoder;
	HR(FactorySingleton::GetImageFactory()->CreateDecoderFromFilename(file, nullptr, GENERIC_READ,
		WICDecodeMetadataCacheOnLoad, decoder.GetAddressOf()));

	ComPtr<IWICBitmapFrameDecode> source;
	HR(decoder->GetFrame(0, source.GetAddressOf()));

	HR(FactorySingleton::GetImageFactory()->CreateFormatConverter(m_BkgdBitmap.ReleaseAndGetAddressOf()));

	HR(m_BkgdBitmap->Initialize(source.Get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone,
		nullptr, 0.0, WICBitmapPaletteTypeMedianCut));

	HR(m_Target->CreateBitmapFromWicBitmap(m_BkgdBitmap.Get(), m_D2DBkgdBitmap.ReleaseAndGetAddressOf()));
}

void CD2DSaveImageDlg::SaveAs(ComPtr<IWICBitmap>& bitmap, PCWSTR filename)
{
	CString filename_lower = filename;
	filename_lower = filename_lower.MakeLower();
	CString ext = filename_lower.Right(4);
	GUID guid = GUID_ContainerFormatPng;
	if (ext == L".png")
		guid = GUID_ContainerFormatPng;
	else if (ext == L".jpg")
		guid = GUID_ContainerFormatJpeg;

	ext = filename_lower.Right(5);
	if (ext == L".jpeg" || ext == L".jpg+")
		guid = GUID_ContainerFormatJpeg;

	ComPtr<IStream> file;

	HR(SHCreateStreamOnFileEx(filename,
		STGM_CREATE | STGM_WRITE | STGM_SHARE_EXCLUSIVE,
		FILE_ATTRIBUTE_NORMAL,
		TRUE, // create
		nullptr, // template
		file.GetAddressOf()));

	ComPtr<IWICBitmapEncoder> encoder;

	HR(FactorySingleton::GetImageFactory()->CreateEncoder(guid,
		nullptr, // vendor
		encoder.GetAddressOf()));

	HR(encoder->Initialize(file.Get(), WICBitmapEncoderNoCache));

	ComPtr<IWICBitmapFrameEncode> frame;
	ComPtr<IPropertyBag2> properties;

	HR(encoder->CreateNewFrame(frame.GetAddressOf(), properties.GetAddressOf()));

	HR(frame->Initialize(properties.Get()));

	UINT width, height;
	HR(bitmap->GetSize(&width, &height));
	HR(frame->SetSize(width, height));

	GUID pixelFormat;
	HR(bitmap->GetPixelFormat(&pixelFormat));

	auto negotiated = pixelFormat;
	HR(frame->SetPixelFormat(&negotiated));

	HR(frame->WriteSource(bitmap.Get(), nullptr));

	HR(frame->Commit());
	HR(encoder->Commit());
}
