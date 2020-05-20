
// D2DBmpRTDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "D2DBmpRT.h"
#include "D2DBmpRTDlg.h"
#include "afxdialogex.h"
#include "../Common/FactorySingleton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CD2DBmpRTDlg dialog



CD2DBmpRTDlg::CD2DBmpRTDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_D2DBMPRT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CD2DBmpRTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CD2DBmpRTDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CD2DBmpRTDlg message handlers

BOOL CD2DBmpRTDlg::OnInitDialog()
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

void CD2DBmpRTDlg::OnPaint()
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
		//CDialogEx::OnPaint();
		CRect rc;
		GetClientRect(rc);
		CPaintDC dc(this);

		if (!m_Target)
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
				m_Target.ReleaseAndGetAddressOf()));

			m_Target->BindDC(dc.GetSafeHdc(), &rc);

			HR(m_Target->CreateCompatibleRenderTarget(m_BmpTarget.ReleaseAndGetAddressOf()));


			CreateDeviceResources();
		}

		Draw();

		ComPtr<ID2D1Bitmap> bitmap;
		m_BmpTarget->GetBitmap(bitmap.GetAddressOf());

		m_Target->BindDC(dc.GetSafeHdc(), &rc);

		m_Target->BeginDraw();

		m_Target->DrawBitmap(bitmap.Get());

		if (D2DERR_RECREATE_TARGET == m_Target->EndDraw())
		{
			m_Target.Reset();
			Invalidate();
		}
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CD2DBmpRTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CD2DBmpRTDlg::CreateDeviceResources()
{
}

void CD2DBmpRTDlg::CreateDeviceIndependentResources()
{
}

void CD2DBmpRTDlg::Draw()
{
	m_BmpTarget->BeginDraw();
	m_BmpTarget->Clear(ColorF(0.26f, 0.56f, 0.87f));
	m_BmpTarget->EndDraw();
}
