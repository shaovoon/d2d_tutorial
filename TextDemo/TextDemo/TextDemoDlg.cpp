
// TextDemoDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "TextDemo.h"
#include "TextDemoDlg.h"
#include "afxdialogex.h"
#include "../Common/FactorySingleton.h"
#include "../Common/ComException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTextDemoDlg dialog



CTextDemoDlg::CTextDemoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TEXTDEMO_DIALOG, pParent)
	, m_pFont(nullptr)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTextDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDT_END_CREDIT, m_edtEndCredit);
	DDX_Control(pDX, IDC_STATIC_TEXT_DISPLAY, m_staticTextDisplay);
	DDX_Control(pDX, IDC_CHK_ITALIC, m_chkItalic);
	DDX_Control(pDX, IDC_CHK_BOLD, m_chkBold);
}

BEGIN_MESSAGE_MAP(CTextDemoDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDT_END_CREDIT, &CTextDemoDlg::OnChangeEdtEndCredit)
	ON_BN_CLICKED(IDC_BTN_END_CREDIT_OK, &CTextDemoDlg::OnBnClickedBtnEndCreditOk)
	ON_BN_CLICKED(IDC_BTN_END_CREDIT_CANCEL, &CTextDemoDlg::OnBnClickedBtnEndCreditCancel)
	ON_BN_CLICKED(IDC_CHK_ITALIC, &CTextDemoDlg::OnBnClickedChkItalic)
	ON_BN_CLICKED(IDC_CHK_BOLD, &CTextDemoDlg::OnBnClickedChkBold)
END_MESSAGE_MAP()


// CTextDemoDlg message handlers

BOOL CTextDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CRect rect(0, 0, 160, 200);
	CRuntimeClass* pClass = RUNTIME_CLASS(CFontScrollView);
	m_pFontScrollView = (CFontScrollView*)pClass->CreateObject();
	m_pFontScrollView->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rect, this, 36001);
	m_pFontScrollView->ModifyStyleEx(0, WS_EX_CLIENTEDGE);

	m_pFontScrollView->OnInitialUpdate();

	UINT dpi = GetDpiForWindow(GetSafeHwnd());

	m_pFontScrollView->SetWindowPos(
		NULL,
		10 * dpi / 96, 30 * dpi / 96, 210 * dpi / 96, 140 * dpi / 96,
		SWP_SHOWWINDOW);

	m_edtEndCredit.SetWindowTextW(L"Hello World");

	LPCTSTR fontName = L"Arial";
	//CreateNewFont(fontName);

	m_pFontScrollView->SetUpdateDrawFunc(std::bind(&CTextDemoDlg::CallDraw, this));

	m_pFontScrollView->SetFontFamily(fontName);

	m_pFontScrollView->ScrollToFont();

	m_edtEndCredit.SetWindowTextW(L"STARRING\r\nJohn Smith\r\nJane Doe");

	CallDraw();


	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTextDemoDlg::OnPaint()
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
		//CPaintDC dc(this); // device context for painting

		m_staticTextDisplay.Invalidate();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTextDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTextDemoDlg::CallDraw()
{
	//Invalidate();
	CString text;
	m_edtEndCredit.GetWindowTextW(text);

	bool italic = (m_chkItalic.GetCheck() == BST_CHECKED);
	bool bold = (m_chkBold.GetCheck() == BST_CHECKED);

	m_staticTextDisplay.SetInfo(m_pFontScrollView->GetFontFamily(), text, true, 1.0f, 26.0f, italic, bold);
	m_staticTextDisplay.Invalidate();
	//CreateNewFont(m_pFontScrollView->GetFontFamily());
}

void CTextDemoDlg::CreateNewFont(LPCTSTR fontName)
{
	if (m_pFont)
		delete m_pFont;

	m_pFont = nullptr;

	m_pFont = new CFont();

	CClientDC dc(this);
	int nFontSize = 20;
	int nHeight = -((dc.GetDeviceCaps(LOGPIXELSY) * nFontSize) / 72);

	m_pFont->CreateFont(nHeight, 0, 0, 0, FW_NORMAL, false, false,
		0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, fontName);

	m_edtEndCredit.SetFont(m_pFont);
}

void CTextDemoDlg::OnChangeEdtEndCredit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CallDraw();
}

void CTextDemoDlg::OnBnClickedBtnEndCreditOk()
{
	// TODO: Add your control notification handler code here
	EndDialog(IDOK);
}


void CTextDemoDlg::OnBnClickedBtnEndCreditCancel()
{
	// TODO: Add your control notification handler code here
	EndDialog(IDCANCEL);
}


void CTextDemoDlg::OnBnClickedChkItalic()
{
	// TODO: Add your control notification handler code here
	CallDraw();
}


void CTextDemoDlg::OnBnClickedChkBold()
{
	// TODO: Add your control notification handler code here
	CallDraw();
}
