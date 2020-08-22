
// D2DImageDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "D2DImage.h"
#include "D2DImageDlg.h"
#include "afxdialogex.h"
#include "../Common/FactorySingleton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CD2DImageDlg dialog



CD2DImageDlg::CD2DImageDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_D2DIMAGE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CD2DImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CD2DImageDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CD2DImageDlg message handlers

BOOL CD2DImageDlg::OnInitDialog()
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
	m_Text = L"HELLO\nWORLD!";

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CD2DImageDlg::OnPaint()
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
HCURSOR CD2DImageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CD2DImageDlg::CreateDeviceResources()
{
}

void CD2DImageDlg::CreateDeviceIndependentResources()
{
	HR(FactorySingleton::GetDWriteFactory()->CreateTextFormat(L"Arial Black",
		nullptr, DWRITE_FONT_WEIGHT_ULTRA_BOLD, DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL, 80, L"",
		m_TextFormat.ReleaseAndGetAddressOf()));
}

void CD2DImageDlg::Draw()
{
	DrawImage();
	//DrawImageText();
}

void CD2DImageDlg::DrawImage()
{
	if (!m_BkgdBitmap)
		LoadImage(m_ImageFile);

	m_Target->Clear(ColorF(ColorF::White));

	if (m_D2DBkgdBitmap)
	{
		auto sizeBmp = m_D2DBkgdBitmap->GetSize();
		auto rectBmp = RectF(0.0f, 0.0f, sizeBmp.width, sizeBmp.height);

		m_Target->DrawBitmap(m_D2DBkgdBitmap.Get(), rectBmp);
	}
}

void CD2DImageDlg::DrawImageText()
{
	if (!m_BkgdBitmap)
	{
		LoadImage(m_PatternImageFile);
		m_Target->CreateBitmapBrush(
			m_D2DBkgdBitmap.Get(),
			m_FillBrush.ReleaseAndGetAddressOf()
		);
	}

	m_Target->Clear(ColorF(ColorF::White));

	if (m_D2DBkgdBitmap)
	{
		auto sizeBmp = m_D2DBkgdBitmap->GetSize();
		auto rectBmp = RectF(0.0f, 0.0f, sizeBmp.width, sizeBmp.height);

		m_Target->DrawTextW((LPCTSTR)m_Text, m_Text.GetLength(), m_TextFormat.Get(), &rectBmp, m_FillBrush.Get());
	}
}

void CD2DImageDlg::LoadImage(LPCTSTR file)
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

