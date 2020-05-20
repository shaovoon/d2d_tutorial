
// D2DWicRTDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "D2DWicRT.h"
#include "D2DWicRTDlg.h"
#include "afxdialogex.h"
#include "../Common/FactorySingleton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CD2DWicRTDlg dialog



CD2DWicRTDlg::CD2DWicRTDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_D2DWICRT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CD2DWicRTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CD2DWicRTDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CD2DWicRTDlg message handlers

BOOL CD2DWicRTDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	PaintAndSaveImage(L"C:\\temp\\sample.PNG");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CD2DWicRTDlg::OnPaint()
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

void CD2DWicRTDlg::PaintAndSaveImage(PCWSTR filename)
{
	CRect rc;
	GetClientRect(rc);

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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CD2DWicRTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CD2DWicRTDlg::CreateDeviceResources()
{
}

void CD2DWicRTDlg::CreateDeviceIndependentResources()
{
}

void CD2DWicRTDlg::Draw()
{
	m_Target->Clear(ColorF(0.26f, 0.56f, 0.87f));
}

void CD2DWicRTDlg::SaveAs(ComPtr<IWICBitmap>& bitmap, PCWSTR filename)
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
