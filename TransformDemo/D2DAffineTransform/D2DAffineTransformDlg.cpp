
// D2DAffineTransformDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "D2DAffineTransform.h"
#include "D2DAffineTransformDlg.h"
#include "afxdialogex.h"
#include "../Common/FactorySingleton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CD2DAffineTransformDlg dialog



CD2DAffineTransformDlg::CD2DAffineTransformDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_D2DAFFINETRANSFORM_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CD2DAffineTransformDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CD2DAffineTransformDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CD2DAffineTransformDlg message handlers

BOOL CD2DAffineTransformDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_Text = L"HELLO WORLD!";
	CreateDeviceIndependentResources();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CD2DAffineTransformDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CD2DAffineTransformDlg::OnPaint()
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

void CD2DAffineTransformDlg::CreateDeviceResources()
{
	HR(m_Target->CreateSolidColorBrush(ColorF(ColorF::Black),
		m_FillBrush.ReleaseAndGetAddressOf()));
}

void CD2DAffineTransformDlg::CreateDeviceIndependentResources()
{
	HR(FactorySingleton::GetDWriteFactory()->CreateTextFormat(L"Arial Black",
		nullptr, DWRITE_FONT_WEIGHT_ULTRA_BOLD, DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL, 40, L"",
		m_TextFormat.ReleaseAndGetAddressOf()));
}

void CD2DAffineTransformDlg::Draw()
{
	m_Target->Clear(ColorF(ColorF::White));

	CRect rectClient;
	GetClientRect(&rectClient);

	auto rect = RectF(0.0f, 0.0f, rectClient.Width(), rectClient.Height());

	m_Target->SetTransform(D2D1::IdentityMatrix());

	D2D1::Matrix3x2F trans = D2D1::Matrix3x2F::Translation(0, 50);
	D2D1::Matrix3x2F rotate = D2D1::Matrix3x2F::Rotation(-10.0f);
	D2D1::Matrix3x2F skew = D2D1::Matrix3x2F::Skew(-10.0f, 0.0f, Point2F(rectClient.Width() / 2, rectClient.Height() / 2));
	D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(0.6f, 0.6f);

	//m_Target->SetTransform(trans);
	//m_Target->SetTransform(trans * skew);
	//m_Target->SetTransform(trans * rotate * skew);
	m_Target->SetTransform(trans * rotate * skew * scale);

	m_FillBrush->SetColor(ColorF(ColorF::Black));
	m_Target->DrawTextW((LPCTSTR)m_Text, m_Text.GetLength(), m_TextFormat.Get(), &rect, m_FillBrush.Get());
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CD2DAffineTransformDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

